#pragma once
#include <string>
#include <sqlite3.h>
#include <iostream>

class Database {
private:
    sqlite3* db; // Pointer to the SQLite database connection

public:
    // Constructor opens the connection
    Database(const std::string& dbPath);
    
    // Destructor ensures the database is safely closed
    ~Database();

    // Creates the "users" table if it doesn't already exist
    void initializeSchema();

    // Inserts a new user into the database
    bool registerUser(const std::string& username, const std::string& passwordHash);

    // Fetches the stored hash to compare during login
    std::string getPasswordHash(const std::string& username);
};