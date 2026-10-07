#include "Player.h"

Player::Player(const std::string& user, const std::string& connId, char sym)
    : username(user), connectionId(connId), symbol(sym) {}

std::string Player::getUsername() const { 
    return username; 
}

std::string Player::getConnectionId() const { 
    return connectionId; 
}

char Player::getSymbol() const { 
    return symbol; 
}
