#include "GameSession.h"

GameSession::GameSession(std::string id, std::string host) 
    : roomId(id), playerOne(host), playerTwo(""), currentTurn('X'), isGameActive(true) {}

bool GameSession::join(std::string guestName) {
    std::lock_guard<std::mutex> lock(mtx);
    if (playerTwo.empty() && guestName != playerOne) {
        playerTwo = guestName;
        return true;
    }
    return false;
}

bool GameSession::makeMove(int index, char symbol) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!isGameActive || (symbol != 'X' && symbol != 'O') || symbol != currentTurn ||
        playerTwo.empty() || !board.makeMove(index, symbol)) {
        return false;
    }

    if (board.checkWin(symbol) || board.isFull()) {
        isGameActive = false;
    } else {
        currentTurn = (currentTurn == 'X') ? 'O' : 'X';
    }
    return true;
}

void GameSession::reset() {
    std::lock_guard<std::mutex> lock(mtx);
    board.reset();
    currentTurn = 'X';
    isGameActive = true;
}
