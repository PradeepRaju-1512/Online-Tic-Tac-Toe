#include "crow.h"
#include "Database.h"
#include "Auth.h"
#include "GameSession.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <string>

int main() {
    crow::SimpleApp app;

    // 1. Initialize persistent data layers
    Database db("tictactoe.db");
    db.initializeSchema();
    Auth auth(db);

    // 2. State management for active multiplayer games
    // Maps a unique Game ID to a specific GameSession object
    std::unordered_map<std::string, std::shared_ptr<GameSession>> activeGames;
    std::mutex gamesMutex; // Prevents race conditions when adding/removing sessions

    // --- HTTP REST API ENDPOINTS (Authentication) ---

    // Handles POST requests to create a new user account
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)
    ([&auth](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid JSON payload");
        }

        std::string user = body["username"].s();
        std::string pass = body["password"].s();

        if (auth.registerAccount(user, pass)) {
            return crow::response(200, "Registration successful");
        }
        return crow::response(409, "Username already exists");
    });

    // Handles POST requests to verify credentials and return a token
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)
    ([&auth](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid JSON payload");
        }

        std::string user = body["username"].s();
        std::string pass = body["password"].s();

        auto loggedInUser = auth.login(user, pass);
        
        if (loggedInUser.has_value()) {
            std::string token = auth.generateSessionToken(user);
            
            // Build a JSON response containing the auth token
            crow::json::wvalue res;
            res["token"] = token;
            res["username"] = loggedInUser->getUsername();
            return crow::response(200, res);
        }
        return crow::response(401, "Invalid username or password");
    });


    // --- WEBSOCKET ENDPOINT (Real-time Matchmaking & Gameplay) ---

    CROW_WEBSOCKET_ROUTE(app, "/game")
    .onopen([&](crow::websocket::connection& conn) {
        CROW_LOG_INFO << "New WebSocket connection established.";
        
        // In a complete implementation, you would:
        // 1. Extract the user's token from the connection URL.
        // 2. Validate the token against active logins.
        // 3. Search `activeGames` for a session with state `WAITING_FOR_PLAYERS`.
        // 4. If found, call `session->joinGame(...)`. If not, create a new GameSession.
    })
    .onclose([&](crow::websocket::connection& conn, const std::string& reason) {
        CROW_LOG_INFO << "WebSocket disconnected: " << reason;
        
        // Locate the user's GameSession and handle the disconnect.
        // Typically, if a player disconnects during IN_PROGRESS, they forfeit the match.
    })
    .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
        auto msg = crow::json::load(data);
        if (!msg) return;

        // Route the network message to the appropriate GameSession
        if (msg.has("action") && msg["action"].s() == "move") {
            int cellIndex = msg["cell"].i();
            std::string connId = std::to_string(reinterpret_cast<uintptr_t>(&conn)); // Unique ID for this connection
            
            // Note: You would lookup the correct GameSession pointer from activeGames here
            // bool success = session->processMove(connId, cellIndex);
            
            // If success is true, iterate over both players in that session 
            // and call conn.send_text(session->getBoardState()) to sync their screens.
        }
    });

    // 3. Boot the multithreaded server on port 9000
    // The .multithreaded() flag allows Crow to handle concurrent REST and WebSocket traffic
    CROW_LOG_INFO << "Server booting on http://localhost:9000";
    app.port(9000).multithreaded().run();
}
