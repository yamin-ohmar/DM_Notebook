
#include <crow.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <utility>

int main() {
    // Open the SQLite database
    sqlite3* db = nullptr;  // pointer to SQLite db connection
    int result = sqlite3_open("database/dm_notebook.db", &db);  // open SQLite db connection

    // If not successful
    if (result != SQLITE_OK) {
        std::cerr << "Failed to open database: "
                  << sqlite3_errmsg(db) << '\n';    // retrieve db error message
        // Close the SQLite database
        sqlite3_close(db);
        return 1;
    }
    // If successful
    std::cout << "SQLite database connected successfully!\n";

    // Create a Crow web application
    crow::SimpleApp app;

    // Define the root ("/") route using a lambda function for GET requests
    // Serve the frontend homepage
    CROW_ROUTE(app, "/")([]() {
        crow::response res;                                 // Creates an HTTP response object
        res.set_static_file_info("frontend/index.html");    // Configures the response to serve a file (differe content types)
        return res;
    });

    // Serve the frontend stylesheet
    CROW_ROUTE(app, "/style.css")([]() {
        crow::response res;
        res.set_static_file_info("frontend/style.css");
        return res;
    });

    // Serve the frontend JavaScript
    CROW_ROUTE(app, "/script.js")([]() {
        crow::response res;
        res.set_static_file_info("frontend/script.js");
        return res;
    });

    // Return all NPCs from SQLite (GET)
    CROW_ROUTE(app, "/api/npcs").methods(crow::HTTPMethod::GET)([]() {
        sqlite3* db = nullptr;

        // Open a read-only connection for this request
        if (sqlite3_open_v2("database/dm_notebook.db", &db,
                            SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Database connection failed");
        }

        // SQL query
        const char* sql =
            "SELECT id, name, race, role, description FROM npcs ORDER BY id;";

        sqlite3_stmt* stmt = nullptr;

        // Prepare the SQL query (compile SQL into a prepared statement)
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare query");
        }

        // Helper function to read text from a SQLite column as a C++ string
        auto getText = [&](int column) -> std::string {
            // Read text from current row
            const unsigned char* text = sqlite3_column_text(stmt, column);
            // Convert SQLite text to a C++ string, or return "" if NULL
            return text ? reinterpret_cast<const char*>(text) : "";
        };

        // Create an empty list to store NPCs as JSON objects
        crow::json::wvalue::list npcs;

        // Process each returned row
        int result;
        while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {   // Executes the statement
            crow::json::wvalue npc;

            npc["id"] = sqlite3_column_int(stmt, 0);    // Reads an integer from the current row
            npc["name"] = getText(1);
            npc["race"] = getText(2);
            npc["role"] = getText(3);
            npc["description"] = getText(4);

            // Move the completed NPC JSON object into the list (avoid copying)
            npcs.push_back(std::move(npc));     // npc is likely empty now
        }

        // Release database resources
        sqlite3_finalize(stmt);     // Releases the prepared statement
        sqlite3_close(db);          // Closes the connection

        // Check whether the SQL query completed successfully (no more rows)
        if (result != SQLITE_DONE) {
            return crow::response(500, "Failed to read NPC records");
        }

        // Convert the NPC list into a Crow JSON value
        crow::json::wvalue json(std::move(npcs));

        // Create an HTTP 200 OK response containing serialized JSON text
        crow::response res(200, json.dump());

        // Tell the browser that the response contains JSON data
        res.set_header("Content-Type", "application/json");

        // Send the response back to the client
        return res;
    });


    // Create a new NPC (POST)
    CROW_ROUTE(app, "/api/npcs").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {

        /********** Getting the new data fron frontend **********/
        // Parse JSON data sent by the frontend
        auto body = crow::json::load(req.body);

        // Validate the request body's name field
        if (!body || !body.has("name") ||
            body["name"].t() != crow::json::type::String) {
            return crow::response(400, "Valid NPC name is required");
        }

        // Retrieve name field
        std::string name = body["name"].s();    // convert JSON value into C++ string

        // Reject empty or whitespace-only names
        if (name.find_first_not_of(" \t\n\r") == std::string::npos) {
            return crow::response(400, "NPC name cannot be empty");
        }

        // Helper function to read optional fields (key), using default values (fallback) if absent
        auto getOptional = [&](const char* key, const std::string& fallback) {
            if (!body.has(key)) // doesn't have values
                return fallback;

            if (body[key].t() != crow::json::type::String)  // value isn't string
                return fallback;

            std::string value = body[key].s();          // convert JSON value into C++ string
            return value.empty() ? fallback : value;    // if empty, set default
        };

        // Retrieve optional fields
        std::string race = getOptional("race", "Unknown");
        std::string role = getOptional("role", "Unknown");
        std::string description = getOptional("description", "None");

        /****************************************************************/


        /********** Sending the new data to backend Database **********/
        // Open SQLite connection for this request (read & write)
        sqlite3* db = nullptr;

        if (sqlite3_open_v2("database/dm_notebook.db", &db,
                            SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Database connection failed");
        }

        // SQL placeholders (?) prevent SQL injection
        const char* sql =
            "INSERT INTO npcs (name, race, role, description) "
            "VALUES (?, ?, ?, ?);";

        sqlite3_stmt* stmt = nullptr;

        // Prepare SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare INSERT");
        }

        // Bind C++ strings to SQL placeholders (1-based indices)
        // c_str() converts a C++ string into a format SQLite can read
        // SQLITE_TRANSIENT tells SQLite to make its own copy of the string
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, race.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_TRANSIENT);

        // Execute INSERT statement
        int result = sqlite3_step(stmt);

        // Release the prepared statement
        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE) {
            sqlite3_close(db);
            return crow::response(500, "Failed to insert NPC");
        }

        // Retrieve the database-generated NPC ID - from the last row
        sqlite3_int64 newId = sqlite3_last_insert_rowid(db);
        sqlite3_close(db);  // close the connection

        /****************************************************************/

        // Return the newly created NPC
        crow::json::wvalue created;
        created["id"] = newId;
        created["name"] = name;
        created["race"] = race;
        created["role"] = role;
        created["description"] = description;

        // Create an HTTP 201 CREATED response containing serialized JSON text
        crow::response res(201, created.dump());

        // Tell the browser that the response contains JSON data
        res.set_header("Content-Type", "application/json");
        return res;
    });


    // Update an existing NPC using its ID (PUT)
    CROW_ROUTE(app, "/api/npcs/<int>").methods(crow::HTTPMethod::PUT)
    ([](const crow::request& req, int id) {

        // Parse the JSON request body
        auto body = crow::json::load(req.body);

        // Validate the required NPC name
        if (!body || !body.has("name") ||
            body["name"].t() != crow::json::type::String) {
            return crow::response(400, "Valid NPC name is required");
        }

        std::string name = body["name"].s();

        // Reject empty or whitespace-only names
        if (name.find_first_not_of(" \t\n\r") == std::string::npos) {
            return crow::response(400, "NPC name cannot be empty");
        }

        // Read optional fields with default values
        auto getOptional = [&](const char* key, const std::string& fallback) {
            if (!body.has(key) ||
                body[key].t() != crow::json::type::String) {
                return fallback;
            }

            std::string value = body[key].s();
            return value.empty() ? fallback : value;
        };

        std::string race = getOptional("race", "Unknown");
        std::string role = getOptional("role", "Unknown");
        std::string description = getOptional("description", "None");

        // Open SQLite database
        sqlite3* db = nullptr;

        if (sqlite3_open_v2("database/dm_notebook.db", &db,
                            SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Database connection failed");
        }

        // Update the NPC matching the given ID
        const char* sql =
            "UPDATE npcs "
            "SET name = ?, race = ?, role = ?, description = ? "
            "WHERE id = ?;";

        sqlite3_stmt* stmt = nullptr;

        // Prepare the UPDATE statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare UPDATE");
        }

        // Bind NPC data to the SQL placeholders
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, race.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, id);

        // Execute the UPDATE statement
        int result = sqlite3_step(stmt);

        // Release the prepared statement
        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE) {
            sqlite3_close(db);
            return crow::response(500, "Failed to update NPC");
        }

        // Check whether an NPC with this ID was found
        int changedRows = sqlite3_changes(db);
        sqlite3_close(db);

        // If no rows has been changed (because not found)
        if (changedRows == 0) {
            return crow::response(404, "NPC not found");
        }

        // Return the updated NPC as JSON
        crow::json::wvalue updated;
        updated["id"] = id;
        updated["name"] = name;
        updated["race"] = race;
        updated["role"] = role;
        updated["description"] = description;

        // Create an HTTP 200 OK response containing serialized JSON text
        crow::response res(200, updated.dump());

        // Tell the browser that the response contains JSON data
        res.set_header("Content-Type", "application/json");
        return res;
    });


    // Delete an NPC using its ID (DELETE)
    CROW_ROUTE(app, "/api/npcs/<int>").methods(crow::HTTPMethod::DELETE)
    ([](int id) {

        // Open SQLite database
        sqlite3* db = nullptr;

        if (sqlite3_open_v2("database/dm_notebook.db", &db,
                            SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Database connection failed");
        }

        // Prepare SQL statement using a parameter placeholder
        const char* sql = "DELETE FROM npcs WHERE id = ?;";
        sqlite3_stmt* stmt = nullptr;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare DELETE");
        }

        // Bind the NPC ID to the first placeholder
        sqlite3_bind_int(stmt, 1, id);

        // Execute the DELETE statement
        int result = sqlite3_step(stmt);

        // Release the prepared statement
        sqlite3_finalize(stmt);

        if (result != SQLITE_DONE) {
            sqlite3_close(db);
            return crow::response(500, "Failed to delete NPC");
        }

        // Check whether an NPC was actually deleted
        int deletedRows = sqlite3_changes(db);
        sqlite3_close(db);

        // If no rows has been deleted (because ID not found)
        if (deletedRows == 0) {
            return crow::response(404, "NPC not found");
        }

        // Return success without a response body (with status code 204: no content)
        return crow::response(204);
    });

    // Start the HTTP server on port 18080
    // Enable multithreading to handle concurrent requests
    app.port(18080).multithreaded().run();

    // Close SQLite db connection
    sqlite3_close(db);

    return 0;
}
