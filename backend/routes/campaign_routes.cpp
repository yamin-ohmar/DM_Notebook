#include "campaign_routes.h"
#include "../database/campaign_repository.h"

#include <exception>
#include <utility>
#include <vector>

// makes this helper private to this file
namespace {

    // Convert validated JSON into a Campaign object
    Campaign parseCampaign(const crow::json::rvalue& body) {
        Campaign campaign{};

        campaign.name = std::string(body["name"].s());

        campaign.description =
            body.has("description") &&
            body["description"].t() == crow::json::type::String
                ? std::string(body["description"].s()) : "";

        return campaign;
    }

}


// Register campaign-related HTTP endpoints
void registerCampaignRoutes(crow::SimpleApp& app) {
    
    // Retrieve all campaigns
    CROW_ROUTE(app, "/api/campaigns").methods(crow::HTTPMethod::GET)
    ([]() {
        try {
            CampaignRepository repository;

            // Retrieve campaigns from SQLite
            std::vector<Campaign> campaigns = repository.getAllCampaigns();

            // Convert campaigns into JSON
            crow::json::wvalue::list campaignList;

            for (const Campaign& campaign : campaigns) {
                crow::json::wvalue jsonCampaign;
                jsonCampaign["id"] = campaign.id;
                jsonCampaign["name"] = campaign.name;
                jsonCampaign["description"] = campaign.description;
                campaignList.push_back(std::move(jsonCampaign));
            }

            // Return JSON with HTTP 200
            crow::json::wvalue json(std::move(campaignList));
            crow::response res(200, json.dump());
            res.set_header("Content-Type", "application/json");
            return res;

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to retrieve campaigns: " << error.what();
            return crow::response(500, "Failed to retrieve campaigns");
        }
    });

    // Create a new campaign
    CROW_ROUTE(app, "/api/campaigns").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);

            // Validate the required campaign name
            if (!body || !body.has("name") ||
                body["name"].t() != crow::json::type::String ||
                std::string(body["name"].s()).empty()) {
                return crow::response(400, "Campaign name is required");
            }

            Campaign campaign = parseCampaign(body);

            CampaignRepository repository;
            Campaign created = repository.createCampaign(campaign);

            // Return the generated campaign ID
            crow::json::wvalue json;
            json["id"] = created.id;

            crow::response res(201, json.dump());
            res.set_header("Content-Type", "application/json");
            return res;

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to create campaign: " << error.what();
            return crow::response(500, "Failed to create campaign");
        }
    });

    // Update an existing campaign
    CROW_ROUTE(app, "/api/campaigns/<int>").methods(crow::HTTPMethod::PUT)
    ([](const crow::request& req, int id) {
        try {
            auto body = crow::json::load(req.body);

            // Validate the required campaign name
            if (!body || !body.has("name") ||
                body["name"].t() != crow::json::type::String ||
                std::string(body["name"].s()).empty()) {
                return crow::response(400, "Campaign name is required");
            }

            Campaign campaign = parseCampaign(body);
            campaign.id = id;

            CampaignRepository repository;
            if (!repository.updateCampaign(campaign)) {
                return crow::response(404, "Campaign not found");
            }

            return crow::response(200, "Campaign updated successfully");

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to update campaign: " << error.what();
            return crow::response(500, "Failed to update campaign");
        }
    });

    // Delete a campaign by ID
    CROW_ROUTE(app, "/api/campaigns/<int>").methods(crow::HTTPMethod::DELETE)
    ([](int id) {
        try {
            CampaignRepository repository;
            if (!repository.deleteCampaign(id)) {
                return crow::response(404, "Campaign not found");
            }
            // 204 No Content: deletion succeeded
            return crow::response(204);

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to delete campaign: " << error.what();
            return crow::response(500, "Failed to delete campaign");
        }
    });

}
