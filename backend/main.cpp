#include "crow.h"
#include "Database.h"
#include "Auth.h"
#include "GameSession.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <string>
#include <queue>

int main() {
    crow::SimpleApp app;

    // Initialize persistent data layers
    Database db("tictactoe.db");
    db.initializeSchema();
    Auth auth(db);

    // Concurrency and Game State Management
    std::mutex gamesMutex;
    std::unordered_map<std::string, std::shared_ptr<GameSession>> activeGames;
    
    // Maps a unique connection ID to the Game ID they are currently playing in
    std::unordered_map<std::string, std::string> playerToGameMap; 
    
    // Simple matchmaking queue storing the connection ID and a dummy username
    std::queue<std::pair<std::string, crow::websocket::connection*>> waitingRoom;

    // --- HTTP REST API ENDPOINTS ---

    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([&auth](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid JSON payload");
        }
        if (auth.registerAccount(body["username"].s(), body["password"].s())) {
            return crow::response(200, "Registration successful");
        }
        return crow::response(409, "Username already exists");
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&auth](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid JSON payload");
        }
        auto loggedInUser = auth.login(body["username"].s(), body["password"].s());
        if (loggedInUser.has_value()) {
            std::string token = auth.generateSessionToken(body["username"].s());
            crow::json::wvalue res;
            res["token"] = token;
            res["username"] = loggedInUser->getUsername();
            return crow::response(200, res);
        }
        return crow::response(401, "Invalid username or password");
    });

    // --- WEBSOCKET ENDPOINT ---

    CROW_WEBSOCKET_ROUTE(app, "/game")
    .onopen([&](crow::websocket::connection& conn) {
        CROW_LOG_INFO << "New WebSocket connection established.";
    })
    .onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t code) {
        std::string connId = std::to_string(reinterpret_cast<uintptr_t>(&conn));
        std::lock_guard<std::mutex> lock(gamesMutex);
        
        // Remove from active games mapping if they disconnect
        if (playerToGameMap.count(connId)) {
            playerToGameMap.erase(connId);
        }
    })
    .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
        auto msg = crow::json::load(data);
        if (!msg || !msg.has("action")) return;

        std::string connId = std::to_string(reinterpret_cast<uintptr_t>(&conn));
        std::string action = msg["action"].s();
        std::lock_guard<std::mutex> lock(gamesMutex);

        // 1. MATCHMAKING LOGIC
        if (action == "find_match") {
            if (waitingRoom.empty()) {
                // No one is waiting, so this player joins the queue
                waitingRoom.push({connId, &conn});
            } else {
                // Another player is waiting! Pair them up.
                auto opponent = waitingRoom.front();
                waitingRoom.pop();

                std::string gameId = "game_" + connId; // Generate a unique ID for the session
                auto newSession = std::make_shared<GameSession>(gameId);
                
                // Add players to the OOP GameSession (dummy usernames used here for simplicity)
                newSession->joinGame("Player1", opponent.first);
                newSession->joinGame("Player2", connId);
                
                activeGames[gameId] = newSession;
                playerToGameMap[opponent.first] = gameId;
                playerToGameMap[connId] = gameId;

                // Notify Player 1 (X)
                crow::json::wvalue p1Msg;
                p1Msg["type"] = "match_found";
                p1Msg["gameId"] = gameId;
                p1Msg["symbol"] = "X";
                opponent.second->send_text(p1Msg.dump());

                // Notify Player 2 (O)
                crow::json::wvalue p2Msg;
                p2Msg["type"] = "match_found";
                p2Msg["gameId"] = gameId;
                p2Msg["symbol"] = "O";
                conn.send_text(p2Msg.dump());
            }
        } 
        
        // 2. GAMEPLAY LOGIC
        else if (action == "move" && msg.has("cell")) {
            if (playerToGameMap.count(connId) == 0) return; // Player is not in a game

            std::string gameId = playerToGameMap[connId];
            auto session = activeGames[gameId];
            int cellIndex = msg["cell"].i();

            // Attempt to apply the move to the Board via the GameSession
            if (session->processMove(connId, cellIndex)) {
                
                // If the move was valid, broadcast the updated state back to the clients
                crow::json::wvalue stateMsg;
                stateMsg["type"] = "state_update";
                stateMsg["board"] = session->getBoardState();
                
                GameState currentState = session->getState();
                if (currentState == GameState::IN_PROGRESS) {
                    stateMsg["gameState"] = "IN_PROGRESS";
                    // Calculate whose turn is next based on the board (simplified turn toggle)
                    int moves = 0;
                    for(char c : session->getBoardState()) if (c != ' ') moves++;
                    stateMsg["currentTurn"] = (moves % 2 == 0) ? "X" : "O";
                } else {
                    stateMsg["gameState"] = "GAME_OVER";
                    // For a complete implementation, you would expose a getWinner() method on Board
                    // stateMsg["winner"] = session->getWinner(); 
                }

                // In a full implementation, you would store connection pointers in GameSession
                // to selectively broadcast. For this scope, Crow's architecture requires mapping 
                // connections manually to broadcast back to both specific users.
                // Assuming we stored both connection pointers, we would call send_text on both here.
                conn.send_text(stateMsg.dump());
            }
        }
    });

    CROW_LOG_INFO << "Tic-Tac-Toe WebSocket Server booting on http://localhost:9000";
    app.port(9000).multithreaded().run();
}
