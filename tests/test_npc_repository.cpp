
#include "../backend/database/npc_repository.h"

#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdio>


// Verify a test condition and report a failure
void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}


int main() {
    // Create a temporary, isolated SQLite database
    const std::string testDb = "test_npcs.db";

    // Remove data left by previous test runs
    std::remove(testDb.c_str());

    // Open the SQLite database
    sqlite3* db = nullptr;
    if (sqlite3_open(testDb.c_str(), &db) != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to create test database");
    }

    // Create the same NPC table as the production database
    const char* sql =
        "CREATE TABLE IF NOT EXISTS npcs ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "race TEXT DEFAULT 'Unknown',"
        "role TEXT DEFAULT 'Unknown',"
        "description TEXT DEFAULT 'None'"
        ");";

    if (sqlite3_exec(db, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to create test table");
    }

    sqlite3_close(db);

    // Use the test database instead of the production database
    NPCRepository repository(testDb);

    // Test 1: a newly created database should contain no NPCs
    std::vector<NPC> npcs = repository.getAllNPCs();
    require(npcs.empty(), "NPC list should be empty at first");
    std::cout << "PASS: New database contains no NPCs\n";

    // Test 2: Create an NPC
    NPC newNPC{};
    newNPC.name = "Dudu";
    newNPC.race = "Goblin";
    newNPC.role = "Guide";
    newNPC.description = "A friendly goblin";

    // Insert the NPC into the test database
    NPC createdNPC = repository.createNPC(newNPC);

    // Verify that SQLite generated a valid ID
    require(createdNPC.id > 0, "There should be a valid ID created");

    // Retrieve all NPCs from the database
    npcs = repository.getAllNPCs();

    // Verify that exactly one NPC exists
    require(npcs.size() == 1, "There should be exactly one NPC exists");

    // Verify that its information was stored correctly
    require(npcs[0].id == createdNPC.id, "ID created should be the same from the list");
    require(npcs[0].name == "Dudu", "NPC name should be Dudu");
    require(npcs[0].race == "Goblin", "NPC race should be goblin");
    require(npcs[0].role == "Guide", "NPC role should be guide");
    require(npcs[0].description == "A friendly goblin", "There should be a NPC description.");
    std::cout << "PASS: NPC created and retrieved successfully\n";

    // Test 3: Update an NPC
    createdNPC.role = "Merchant";

    // Update the NPC in the test database
    bool npcUpdated = repository.updateNPC(createdNPC);

    // Verify that SQLite updated the NPC
    require(npcUpdated, "Updating an existing NPC should succeed");

    // Retrieve all NPCs from the database
    npcs = repository.getAllNPCs();

    // Verify that exactly one NPC exists
    require(npcs.size() == 1, "There should be exactly one NPC exists");

    // Verify that its information was stored correctly
    require(npcs[0].id == createdNPC.id, "ID created should be the same from the list");
    require(npcs[0].name == "Dudu", "NPC name should be Dudu");
    require(npcs[0].race == "Goblin", "NPC race should be goblin");
    require(npcs[0].role == "Merchant", "NPC role should be merchant");
    require(npcs[0].description == "A friendly goblin", "There should be a NPC description.");
    std::cout << "PASS: NPC updated successfully\n";

    // Test 4: Delete an NPC
    bool npcDeleted = repository.deleteNPC(createdNPC.id);

    // Verify that SQLite deleted the NPC
    require(npcDeleted, "Deleting an existing NPC should succeed");

    // Retrieve all NPCs from the database
    npcs = repository.getAllNPCs();

    // Verify that the NPC list is empty
    require(npcs.empty(), "NPC list should be empty after deletion");
    std::cout << "PASS: NPC deleted successfully\n";

    // Test 5: Delete an non-existing NPC
    bool npcDeleteAgain = repository.deleteNPC(createdNPC.id);

    // Verify that function return false
    require(!npcDeleteAgain, "Deleting a nonexistent NPC should return false");
    std::cout << "PASS: Nonexistent NPC deletion handled correctly\n";

    return 0;
}
