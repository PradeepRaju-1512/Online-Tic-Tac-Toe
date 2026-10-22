#ifndef BOARD_H
#define BOARD_H
#include <string>
using namespace std;
class Board{
private:
    char cells[9];
public:
    Board();
    bool makeMove(int position,char symbol);
    bool checkWin(char symbol) const;
    bool isDraw() const;
    bool isFull() const;
    void reset();
    string getState() const;
};
#endif