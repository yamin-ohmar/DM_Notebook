
#include <crow.h>  // Crow framework for building HTTP servers and REST APIs

int main() {
    // Create a Crow web application
    crow::SimpleApp app;

    // Define the root ("/") route using a lambda function
    // Executes when a client sends a GET request to "/"
    CROW_ROUTE(app, "/")([]() {
        return "Welcome to DM's Notebook!";
    });

    // Handle GET requests to retrieve NPCs
    CROW_ROUTE(app, "/api/npcs")([]() {
        return "NPC endpoint is working!";
    });

    // Start the HTTP server on port 18080
    // Enable multithreading to handle concurrent requests
    app.port(18080).multithreaded().run();

    return 0;
}
