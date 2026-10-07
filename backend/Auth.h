#pragma once
#include "Database.h"
#include "User.h"
#include <string>
#include <optional>

class Auth {
private:
    Database& db; // Reference to the active database connection

    // Helper function to hash passwords before storing/comparing
    std::string hashPassword(const std::string& password);

public:
    // Dependency injection: Auth requires an existing database to function
    Auth(Database& database);

    // Validates inputs and delegates insertion to the Database class
    bool registerAccount(const std::string& username, const std::string& password);

    // Checks credentials and returns a populated User object if successful
    std::optional<User> login(const std::string& username, const std::string& password);
    
    // Generates a unique token for the frontend to use in WebSocket connections
    std::string generateSessionToken(const std::string& username);
};
