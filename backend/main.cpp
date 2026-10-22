
#include <crow.h>
#include "routes/npc_routes.h"

int main() {
    // Create a Crow web application
    crow::SimpleApp app;

    // Register NPC API endpoints
    registerNPCRoutes(app);

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

    // Start the HTTP server on port 18080
    app.port(18080).multithreaded().run();

    return 0;
}
