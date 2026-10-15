
#include <crow.h>  // Crow framework for building HTTP servers and REST APIs

int main() {
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

    // Return all NPCs as JSON (GET)
    CROW_ROUTE(app, "/api/npcs").methods(crow::HTTPMethod::GET)([]() {
        crow::json::wvalue npcs;        // Crow's writable JSON value type

        npcs[0]["id"] = 1;
        npcs[0]["name"] = "Dudu";
        npcs[0]["race"] = "Goblin";
        npcs[0]["role"] = "Guide";
        npcs[0]["description"] = "A friendly child goblin who helps adventurers find their way.";

        npcs[1]["id"] = 2;
        npcs[1]["name"] = "Oliver";
        npcs[1]["race"] = "Copper Dragon";
        npcs[1]["role"] = "Hatchling";
        npcs[1]["description"] = "A young dragon who loves collecting shiny treasures.";

        return npcs;
    });


    // Start the HTTP server on port 18080
    // Enable multithreading to handle concurrent requests
    app.port(18080).multithreaded().run();

    return 0;
}
