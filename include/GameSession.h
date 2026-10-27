#pragma once
#include "Board.h"
#include <string>
#include <mutex>

class GameSession {
public:
    GameSession(std::string roomId, std::string hostName);
    
    std::string roomId;
    std::string playerOne;
    std::string playerTwo;
    Board board;
    char currentTurn;
    bool isGameActive;
    std::mutex mtx;

    bool join(std::string guestName);
    bool makeMove(int index, char symbol);
    void reset();
};