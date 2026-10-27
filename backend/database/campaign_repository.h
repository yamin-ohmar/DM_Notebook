#pragma once

#include <string>
#include <vector>
#include "../models/campaign.h"

class CampaignRepository {
public:
    // Default database path for the application
    explicit CampaignRepository(
        const std::string& dbPath = "database/dm_notebook.db"
    );

    // Retrieve all campaigns from SQLite
    std::vector<Campaign> getAllCampaigns();

    // Insert a campaign and return it with its generated ID
    Campaign createCampaign(const Campaign& campaign);

    // Update an existing campaign; return false if ID doesn't exist
    bool updateCampaign(const Campaign& campaign);

    // Delete a campaign by ID; return false if ID doesn't exist
    bool deleteCampaign(int id);

private:
    std::string dbPath;
    
};
