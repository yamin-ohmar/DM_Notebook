
#include "../backend/database/campaign_repository.h"

#include <sqlite3.h>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Verify a condition and throw an exception if it fails
void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

int main() {
    // Use a separate database for campaign tests
    const std::string testDb = "test_campaigns.db";

    // Remove data from previous test runs
    std::remove(testDb.c_str());

    // Create the SQLite test database
    sqlite3* db = nullptr;
    if (sqlite3_open(testDb.c_str(), &db) != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to create test database");
    }

    // Create the campaigns table
    const char* sql =
        "CREATE TABLE IF NOT EXISTS campaigns ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "description TEXT DEFAULT ''"
        ");";

    if (sqlite3_exec(db, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to create campaigns table");
    }

    sqlite3_close(db);

    // Connect the repository to the test database
    CampaignRepository repository(testDb);

    // Test 1: A new database should contain no campaigns
    std::vector<Campaign> campaigns = repository.getAllCampaigns();
    require(campaigns.empty(), "Campaign list should initially be empty");
    std::cout << "PASS: New database contains no campaigns\n";

    // Test 2: Create a campaign
    Campaign newCampaign{};
    newCampaign.name = "The Keep on the Borderlands";
    newCampaign.description = "Exploring the Caves of Chaos";

    // Insert the campaign into the test database
    Campaign createdCampaign = repository.createCampaign(newCampaign);
    
    // Verify that SQLite generated a valid ID
    require(createdCampaign.id > 0, "There should be a valid ID created");

    // Retrieve all campaigns from the database
    campaigns = repository.getAllCampaigns();

    // Verify that exactly one campaign exists
    require(campaigns.size() == 1, "There should be exactly one campaign exists");

    // Verify that its information was stored correctly
    require(campaigns[0].id == createdCampaign.id, "ID created should be the same from the list");
    require(campaigns[0].name == "The Keep on the Borderlands", "Campaign name should be The Keep on the Borderlands");
    require(campaigns[0].description == "Exploring the Caves of Chaos", "There should be a campaign description.");
    std::cout << "PASS: Campaign created and retrieved successfully\n";

    // Test 3: Update a campaign
    createdCampaign.description = "Exploring the Keep";

    // Update the campaign in the test database
    bool campaignUpdated = repository.updateCampaign(createdCampaign);

    // Verify that SQLite updated the campaign
    require(campaignUpdated, "Updating an existing campaign should succeed");

    // Retrieve all campaigns from the database
    campaigns = repository.getAllCampaigns();

    // Verify that exactly one campaign exists
    require(campaigns.size() == 1, "There should be exactly one campaign exists");

    // Verify that its information was stored correctly
    require(campaigns[0].id == createdCampaign.id, "ID created should be the same from the list");
    require(campaigns[0].name == "The Keep on the Borderlands", "Campaign name should be The Keep on the Borderlands");
    require(campaigns[0].description == "Exploring the Keep", "There should be an updated campaign description.");
    std::cout << "PASS: Campaign updated successfully\n";

    // Test 4: Update a non-existing campaign
    Campaign nonCampaign{};
    nonCampaign.id = 99999; // An ID that doesn't exist
    nonCampaign.name = "Caves of Chaos";
    nonCampaign.description = "Exploring the Caves of Chaos";

    // Update the campaign in the test database
    bool campaignUpdateAgain = repository.updateCampaign(nonCampaign);

    // Verify that SQLite updated the campaign
    require(!campaignUpdateAgain, "Updating non-existing campaign should return false");
    std::cout << "PASS: Nonexistent campaign update handled correctly\n";

    // Test 5: Delete a campaign
    bool campaignDeleted = repository.deleteCampaign(createdCampaign.id);

    // Verify that SQLite deleted the campaign
    require(campaignDeleted, "Deleting an existing campaign should succeed");

    // Retrieve all campaigns from the database
    campaigns = repository.getAllCampaigns();

    // Verify that the campaign list is empty
    require(campaigns.empty(), "Campaign list should be empty after deletion");
    std::cout << "PASS: Campaign deleted successfully\n";

    // Test 6: Delete an non-existing campaign
    bool campaignDeleteAgain = repository.deleteCampaign(createdCampaign.id);

    // Verify that function return false
    require(!campaignDeleteAgain, "Deleting a nonexistent campaign should return false");
    std::cout << "PASS: Nonexistent campaign deletion handled correctly\n";

    return 0;
}
