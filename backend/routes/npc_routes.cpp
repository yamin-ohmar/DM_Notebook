
#include "npc_routes.h"
#include "../database/npc_repository.h"

#include <exception>
#include <utility>

// keeping the helper functions private inside namespace
namespace {
    // Convert validated JSON data into an NPC object
    NPC parseNPC(const crow::json::rvalue& body) {
        NPC npc{};

        npc.name = std::string(body["name"].s());

        npc.race = body.has("race") &&
                   body["race"].t() == crow::json::type::String
            ? std::string(body["race"].s()) : "Unknown";

        npc.role = body.has("role") &&
                   body["role"].t() == crow::json::type::String
            ? std::string(body["role"].s()) : "Unknown";

        npc.description = body.has("description") &&
                          body["description"].t() == crow::json::type::String
            ? std::string(body["description"].s()) : "None";

        return npc;
    }

}


// Register NPC-related HTTP endpoints
void registerNPCRoutes(crow::SimpleApp& app) {

    // Retrieve all NPCs from the database (GET)
    CROW_ROUTE(app, "/api/npcs").methods(crow::HTTPMethod::GET)
    ([]() {
        try {
            NPCRepository repository;

            // Retrieve NPC objects from SQLite
            std::vector<NPC> npcs = repository.getAllNPCs();

            // Convert NPC objects into a JSON array
            crow::json::wvalue::list npcList;

            for (const NPC& npc : npcs) {
                crow::json::wvalue jsonNPC;

                jsonNPC["id"] = npc.id;
                jsonNPC["name"] = npc.name;
                jsonNPC["race"] = npc.race;
                jsonNPC["role"] = npc.role;
                jsonNPC["description"] = npc.description;

                npcList.push_back(std::move(jsonNPC));
            }

            // Return the JSON array with HTTP 200 OK
            crow::json::wvalue json(std::move(npcList));
            crow::response res(200, json.dump());
            res.set_header("Content-Type", "application/json");
            return res;

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to retrieve NPCs: " << error.what();
            return crow::response(500, "Failed to retrieve NPCs");
        }
    });


    // Create a new NPC (POST)
    CROW_ROUTE(app, "/api/npcs").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);

            // Validate the required name field
            if (!body || !body.has("name") ||
                body["name"].t() != crow::json::type::String ||
                std::string(body["name"].s()).empty()) {
                return crow::response(400, "NPC name is required");
            }

            // Convert JSON into an NPC object
            NPC npc = parseNPC(body);
            NPCRepository repository;
            NPC created = repository.createNPC(npc);

            // Return the generated ID and HTTP 201 Created
            crow::json::wvalue json;
            json["id"] = created.id;

            crow::response res(201, json.dump());
            res.set_header("Content-Type", "application/json");
            return res;

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to create NPC: " << error.what();
            return crow::response(500, "Failed to create NPC");
        }
    });


    // Update an existing NPC (PUT)
    CROW_ROUTE(app, "/api/npcs/<int>").methods(crow::HTTPMethod::PUT)
    ([](const crow::request& req, int id) {
        try {
            auto body = crow::json::load(req.body);

            // Validate the required name field
            if (!body || !body.has("name") ||
                body["name"].t() != crow::json::type::String ||
                std::string(body["name"].s()).empty()) {
                return crow::response(400, "NPC name is required");
            }

            // Convert JSON into an NPC object
            NPC npc = parseNPC(body);
            npc.id = id;
            NPCRepository repository;

            // Return 404 if no NPC has the requested ID
            if (!repository.updateNPC(npc)) {
                return crow::response(404, "NPC not found");
            }

            return crow::response(200, "NPC updated successfully");

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to update NPC: " << error.what();
            return crow::response(500, "Failed to update NPC");
        }
    });


    // Delete an NPC by ID (DELETE)
    CROW_ROUTE(app, "/api/npcs/<int>").methods(crow::HTTPMethod::DELETE)
    ([](int id) {
        try {
            NPCRepository repository;

            // Return 404 if the NPC does not exist
            if (!repository.deleteNPC(id)) {
                return crow::response(404, "NPC not found");
            }

            // 204 No Content: deletion succeeded
            return crow::response(204);

        } catch (const std::exception& error) {
            CROW_LOG_ERROR << "Failed to delete NPC: " << error.what();
            return crow::response(500, "Failed to delete NPC");
        }
    });

}
