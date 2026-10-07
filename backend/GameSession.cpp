#include "GameSession.h"

GameSession::GameSession(const std::string& id) 
    : sessionId(id), currentState(GameState::WAITING_FOR_PLAYERS), currentTurn('X') {}

bool GameSession::joinGame(const std::string& username, const std::string& connectionId) {
    std::lock_guard<std::mutex> lock(sessionMutex); // Lock for thread safety

    if (!playerX) {
        playerX = std::make_unique<Player>(username, connectionId, 'X');
        return true;
    } else if (!playerO) {
        playerO = std::make_unique<Player>(username, connectionId, 'O');
        currentState = GameState::IN_PROGRESS; // Game starts when Player 2 connects
        return true;
    }
    return false; // Reject connection if the session already has 2 players
}

bool GameSession::processMove(const std::string& connectionId, int cellIndex) {
    std::lock_guard<std::mutex> lock(sessionMutex); // Prevent race conditions

    if (currentState != GameState::IN_PROGRESS) return false;

    // Identify which player sent the network command
    char activeSymbol = ' ';
    if (playerX && playerX->getConnectionId() == connectionId) activeSymbol = 'X';
    else if (playerO && playerO->getConnectionId() == connectionId) activeSymbol = 'O';

    // Validate that it is actually this player's turn
    if (activeSymbol != ' ' && activeSymbol == currentTurn) {
        
        // Attempt to apply the move to the Board
        if (board.makeMove(cellIndex, activeSymbol)) {
            
            // Check if this move ended the game
            char winState = board.checkWinCondition();
            if (winState != ' ') {
                currentState = GameState::GAME_OVER;
            } else {
                // Pass the turn to the opponent
                currentTurn = (currentTurn == 'X') ? 'O' : 'X';
            }
            return true;
        }
    }
    return false; // Move was invalid (e.g., cell occupied or not their turn)
}

GameState GameSession::getState() const {
    std::lock_guard<std::mutex> lock(sessionMutex);
    return currentState;
}

std::string GameSession::getSessionId() const {
    return sessionId; // Read-only on creation, no lock needed
}

std::string GameSession::getBoardState() const {
    std::lock_guard<std::mutex> lock(sessionMutex);
    return board.serialize();
}
