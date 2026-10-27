#pragma once
#include <vector>
#include <string>

class Board {
public:
    Board();
    void reset();
    bool makeMove(int index, char symbol);
    bool checkWin(char symbol) const;
    bool isFull() const;
    std::vector<std::string> getState() const;

private:
    std::vector<char> cells;
};