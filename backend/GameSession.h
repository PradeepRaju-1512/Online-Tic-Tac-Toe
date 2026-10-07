#pragma once
#include "Board.h"
#include "Player.h"
#include <memory>
#include <mutex>
#include <string>

// State machine for the game lifecycle
enum class GameState {
    WAITING_FOR_PLAYERS,
    IN_PROGRESS,
    GAME_OVER
};

class GameSession {
private:
    std::string sessionId;
    Board board;
    
    // Using unique_ptr so the Player object is strictly owned by this session
    std::unique_ptr<Player> playerX;
    std::unique_ptr<Player> playerO;
    
    GameState currentState;
    char currentTurn; // Tracks whose turn it is: 'X' or 'O'
    
    // Mutex to protect game state from concurrent WebSocket thread access
    mutable std::mutex sessionMutex; 

public:
    GameSession(const std::string& id);

    // Attempts to add a player to this specific session
    bool joinGame(const std::string& username, const std::string& connectionId);

    // Processes an incoming move from the network
    bool processMove(const std::string& connectionId, int cellIndex);

    // Thread-safe getters
    GameState getState() const;
    std::string getSessionId() const;
    std::string getBoardState() const;
};
