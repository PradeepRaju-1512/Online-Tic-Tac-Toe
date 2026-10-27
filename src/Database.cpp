#include "Database.h"
#include <cstdlib>
#include <iostream>
#include <pqxx/pqxx>

Database::Database() {
    const char* dbPasswordEnv = std::getenv("DATABASE_PASSWORD");
    std::string dbPassword = dbPasswordEnv ? dbPasswordEnv : "secure_password_123";

    const char* dbHostEnv = std::getenv("DB_HOST");
    std::string dbHost = dbHostEnv ? dbHostEnv : "localhost";

    connectionString = "dbname=tictactoe_db user=postgres password=" + dbPassword + " host=" + dbHost + " port=5432";
}

bool Database::connect() {
    try {
        // Attempt to establish a live connection using libpqxx
        pqxx::connection conn(connectionString);
        if (conn.is_open()) {
            std::cout << "[PostgreSQL] Successfully connected to database: " << conn.dbname() << std::endl;
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "[PostgreSQL Error] Connection failed: " << e.what() << std::endl;
    }
    return false;
}

void Database::logMatch(const std::string& roomId, const std::string& p1, const std::string& p2, const std::string& winner) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);
        txn.exec(
            "INSERT INTO match_logs (room_id, player1, player2, winner) VALUES ($1, $2, $3, $4);",
            pqxx::params{roomId, p1, p2, winner}
        );
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "[PostgreSQL Error] Failed to log match: " << e.what() << std::endl;
    }
}

std::string Database::getLeaderboardJson() {
    try {
        pqxx::connection conn(connectionString);
        pqxx::read_transaction txn(conn);
        pqxx::result res = txn.exec("SELECT winner, COUNT(*) as wins FROM match_logs WHERE winner != 'Draw' GROUP BY winner ORDER BY wins DESC LIMIT 10;");
        
        std::string json = "[";
        for (auto row : res) {
            if (json.length() > 1) json += ",";
            json += "{\"winner\":\"" + row["winner"].as<std::string>() + "\",\"wins\":" + row["wins"].as<std::string>() + "}";
        }
        json += "]";
        return json;
    } catch (const std::exception& e) {
        std::cerr << "[PostgreSQL Error] Failed to fetch leaderboard: " << e.what() << std::endl;
        return "[{\"error\":\"Database query failed\"}]";
    }
}