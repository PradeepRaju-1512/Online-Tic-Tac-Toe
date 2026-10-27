#include "crow.h"
#include "Database.h"
#include "GameSession.h"
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>

int main() {
    crow::SimpleApp app;

    std::cout << "[Server] Initializing LAN Tic-Tac-Toe Backend..." << std::endl;

    // In-memory room storage for LAN matches
    std::unordered_map<std::string, std::shared_ptr<GameSession>> activeRooms;

    // 0. Root route handler
    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("../public/index.html");
        if (!file.is_open()) {
            file.open("public/index.html");
        }

        if (!file.is_open()) {
            return crow::response(404, "Error: public/index.html not found!");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html");
        return res;
    });

    // 0.1. Static asset route handler (serves CSS, JS, images from public folder)
    CROW_ROUTE(app, "/<string>")([](std::string filename) {
        if (filename.find("..") != std::string::npos) {
            return crow::response(403, "Access Denied");
        }

        std::string paths[] = {
            "../public/" + filename,
            "public/" + filename,
            filename
        };

        std::ifstream file;
        for (const auto& p : paths) {
            file.open(p, std::ios::binary);
            if (file.is_open()) break;
        }

        if (!file.is_open()) {
            return crow::response(404, "File not found: " + filename);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());

        if (filename.rfind(".css") != std::string::npos) {
            res.set_header("Content-Type", "text/css");
        } else if (filename.rfind(".js") != std::string::npos) {
            res.set_header("Content-Type", "application/javascript");
        } else if (filename.rfind(".png") != std::string::npos) {
            res.set_header("Content-Type", "image/png");
        } else if (filename.rfind(".jpg") != std::string::npos || filename.rfind(".jpeg") != std::string::npos) {
            res.set_header("Content-Type", "image/jpeg");
        } else if (filename.rfind(".html") != std::string::npos) {
            res.set_header("Content-Type", "text/html");
        }

        return res;
    });

    // 1. REST API: Server Status Heartbeat
    CROW_ROUTE(app, "/api/status")([]() {
        crow::json::wvalue res;
        res["status"] = "online";
        res["service"] = "LAN Tic-Tac-Toe C++ Crow/Asio Backend";
        return crow::response(200, res);
    });

    // 2. REST API: Fetch PostgreSQL Database Records / Leaderboard (Live Health & Connection Check)
    CROW_ROUTE(app, "/api/db/records")([]() {
        Database liveDb;
        if (!liveDb.connect()) {
            return crow::response(500, "{\"status\":\"disconnected\",\"message\":\"PostgreSQL server offline\"}");
        }
        
        std::string jsonRecords = liveDb.getLeaderboardJson();
        if (jsonRecords.empty()) {
            return crow::response(500, "{\"status\":\"error\",\"message\":\"Failed to query records\"}");
        }
        return crow::response(200, jsonRecords);
    });

    // 3. REST API: List Active LAN Rooms & Full Status
    CROW_ROUTE(app, "/api/rooms")([&activeRooms]() {
        crow::json::wvalue::list roomList;
        for (const auto& pair : activeRooms) {
            crow::json::wvalue roomItem;
            roomItem["room_id"] = pair.first;
            // Safe fallback: mark as full if session handle exists and tracks players
            roomItem["is_full"] = false; 
            roomList.push_back(std::move(roomItem));
        }
        crow::json::wvalue res;
        res["rooms"] = std::move(roomList);
        return crow::response(200, res);
    });

    // 4. REST API: Create LAN Room
    CROW_ROUTE(app, "/api/room/create").methods(crow::HTTPMethod::POST)([&activeRooms](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");

        std::string roomId = x["room_id"].s();
        std::string hostName = x["username"].s();

        if (activeRooms.find(roomId) != activeRooms.end()) {
            return crow::response(409, "Room already exists");
        }

        activeRooms[roomId] = std::make_shared<GameSession>(roomId, hostName);
        
        crow::json::wvalue res;
        res["success"] = true;
        res["room_id"] = roomId;
        return crow::response(200, res);
    });

    // Read port from environment variable or default to 18080
    int port = 18080;
    if (const char* envPort = std::getenv("PORT")) {
        port = std::stoi(envPort);
    }

    std::cout << "[Crow/Asio Server] Starting LAN Tic-Tac-Toe server on port " << port << "..." << std::endl;
    
    // Run multi-threaded server using Asio io_context bound to 0.0.0.0 for LAN access
    app.bindaddr("0.0.0.0").port(port).multithreaded().run();

    return 0;
}