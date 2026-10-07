#include "Auth.h"
#include <functional> // For std::hash
#include <chrono>     // For generating token timestamps

Auth::Auth(Database& database) : db(database) {}

std::string Auth::hashPassword(const std::string& password) {
    // WARNING: std::hash is NOT secure against brute-force attacks.
    // Replace this logic with bcrypt if you ever deploy this to production.
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

bool Auth::registerAccount(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) return false;
    
    std::string hashedPw = hashPassword(password);
    return db.registerUser(username, hashedPw);
}

std::optional<User> Auth::login(const std::string& username, const std::string& password) {
    std::string storedHash = db.getPasswordHash(username);
    
    // If the database returns an empty string, the user does not exist
    if (storedHash.empty()) {
        return std::nullopt; 
    }
    
    std::string inputHash = hashPassword(password);
    
    // Validate password
    if (storedHash == inputHash) {
        // In a fully expanded project, you would execute a SELECT query here 
        // to grab the user's actual wins/losses. For now, we return default stats.
        return User(username, 0, 0); 
    }
    
    return std::nullopt; // Password mismatch
}

std::string Auth::generateSessionToken(const std::string& username) {
    // Generates a pseudo-random string (e.g., "Player1_1697042539000")
    // The frontend will save this token in localStorage and pass it to the WebSocket.
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    return username + "_" + std::to_string(now);
}
