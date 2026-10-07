#pragma once
#include <string>

class Player {
private:
    std::string username;
    std::string connectionId; // Unique handle for the active WebSocket connection
    char symbol;              // 'X' or 'O'

public:
    // Constructor initializes the player with their connection details
    Player(const std::string& user, const std::string& connId, char sym);

    // Getters for player metadata
    std::string getUsername() const;
    std::string getConnectionId() const;
    char getSymbol() const;
};
