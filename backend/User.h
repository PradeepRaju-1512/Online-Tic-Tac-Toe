#pragma once
#include <string>

class User {
private:
    std::string username;
    int wins;
    int losses;

public:
    // Constructor with default values for new players
    User(const std::string& user, int w = 0, int l = 0);

    // Read-only accessors
    std::string getUsername() const;
    int getWins() const;
    int getLosses() const;
    
    // Mutators for updating stats after a game ends
    void addWin();
    void addLoss();
};
