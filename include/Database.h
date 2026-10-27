#pragma once
#include <string>
#include <pqxx/pqxx>

class Database {
public:
    Database();
    bool connect();
    void logMatch(const std::string& roomId, const std::string& p1, const std::string& p2, const std::string& winner);
    std::string getLeaderboardJson();

private:
    std::string connectionString;
};