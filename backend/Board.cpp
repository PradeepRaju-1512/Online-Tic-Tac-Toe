#include "Board.h"

Board::Board() : grid(9, ' ') {}

bool Board::makeMove(int index, char symbol) {
    // Validate index boundaries and check if the cell is empty
    if (index >= 0 && index < 9 && grid[index] == ' ') {
        grid[index] = symbol;
        return true;
    }
    return false;
}

char Board::checkWinCondition() const {
    // Define all 8 possible winning line combinations (indices)
    const int winLines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Cols
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    // Check for a winner
    for (const auto& line : winLines) {
        if (grid[line[0]] != ' ' && 
            grid[line[0]] == grid[line[1]] && 
            grid[line[1]] == grid[line[2]]) {
            return grid[line[0]]; // Returns 'X' or 'O'
        }
    }

    // Check for a draw (no empty spaces left)
    for (char cell : grid) {
        if (cell == ' ') return ' '; // Game is still ongoing
    }

    return 'D'; // Draw
}

std::string Board::serialize() const {
    // Converts the vector into a 9-character string (e.g., "XX O  O X")
    return std::string(grid.begin(), grid.end());
}
