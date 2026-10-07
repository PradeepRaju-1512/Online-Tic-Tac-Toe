#pragma once
#include <vector>
#include <string>

class Board {
private:
    std::vector<char> grid; // Represents the 9 cells. ' ' means empty.

public:
    Board();

    // Attempts to place 'X' or 'O' at the given index (0-8)
    bool makeMove(int index, char symbol);

    // Evaluates the board. Returns 'X' or 'O' for a win, 'D' for draw, ' ' for ongoing.
    char checkWinCondition() const;

    // Converts the board to a simple string to send over WebSockets
    std::string serialize() const;
};
