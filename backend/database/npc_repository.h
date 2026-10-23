#pragma once

#include <string>
#include <vector>
#include "../models/npc.h"

class NPCRepository {
public:
    // Default database path for the application
    explicit NPCRepository(
        const std::string& dbPath = "database/dm_notebook.db"
    );

    // Retrieve all NPCs from SQLite
    std::vector<NPC> getAllNPCs();

    // Insert an NPC and return it with its generated ID
    NPC createNPC(const NPC& npc);

    // Update an existing NPC; return false if ID doesn't exist
    bool updateNPC(const NPC& npc);

    // Delete an NPC by ID; return false if ID doesn't exist
    bool deleteNPC(int id);

private:
    std::string dbPath;
    
};
