#include "User.h"

User::User(const std::string& user, int w, int l) 
    : username(user), wins(w), losses(l) {}

std::string User::getUsername() const { 
    return username; 
}

int User::getWins() const { 
    return wins; 
}

int User::getLosses() const { 
    return losses; 
}

void User::addWin() { 
    wins++; 
}

void User::addLoss() { 
    losses++; 
}
