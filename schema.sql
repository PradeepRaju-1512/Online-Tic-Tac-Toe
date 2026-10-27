-- Create the main database
CREATE DATABASE tictactoe_db;

-- Connect to tictactoe_db and run the following:

-- Table for tracking registered users, usernames, and scores
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    wins INT DEFAULT 0,
    losses INT DEFAULT 0,
    draws INT DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table for logging completed matches (End-to-End integration requirement)
CREATE TABLE IF NOT EXISTS match_logs (
    id SERIAL PRIMARY KEY,
    room_id VARCHAR(50) NOT NULL,
    player_one VARCHAR(50) NOT NULL,
    player_two VARCHAR(50) NOT NULL,
    winner VARCHAR(50),
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Seed initial test user record for the leaderboard console
INSERT INTO users (username, wins, losses, draws) 
VALUES ('Player_One', 12, 3, 1) 
ON CONFLICT (username) DO NOTHING;