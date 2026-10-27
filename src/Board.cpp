#include "Board.h"
#include <algorithm>

Board::Board() : cells(9, ' ') {}

void Board::reset() {
    std::fill(cells.begin(), cells.end(), ' ');
}

bool Board::makeMove(int index, char symbol) {
    if (index >= 0 && index < 9 && cells[index] == ' ') {
        cells[index] = symbol;
        return true;
    }
    return false;
}

bool Board::checkWin(char symbol) const {
    const int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    for (auto& w : wins) {
        if (cells[w[0]] == symbol && cells[w[1]] == symbol && cells[w[2]] == symbol)
            return true;
    }
    return false;
}

bool Board::isFull() const {
    for (char c : cells) {
        if (c == ' ') return false;
    }
    return true;
}

std::vector<std::string> Board::getState() const {
    std::vector<std::string> s;
    for (char c : cells) s.push_back(std::string(1, c));
    return s;
}
