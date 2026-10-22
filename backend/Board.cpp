#include "Board.h"
Board::Board(){
    reset();
}
void Board::reset(){
    for(int i=0;i<9;i++) cells[i]=' ';
}
bool Board::makeMove(int position,char symbol){
    if(position<0||position>8||cells[position]!=' '||(symbol!='X'&&symbol!='O')) return false;
    cells[position]=symbol;
    return true;
}
bool Board::checkWin(char s) const{
    int lines[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(auto &line:lines)
        if(cells[line[0]]==s&&cells[line[1]]==s&&cells[line[2]]==s) return true;
    return false;
}
bool Board::isFull() const{
    for(char c:cells) if(c==' ') return false;
    return true;
}
bool Board::isDraw() const{
    return isFull()&&!checkWin('X')&&!checkWin('O');
}
string Board::getState() const{
    string result;
    for(int i=0;i<9;i++){
        result+=cells[i];
        if(i<8) result+=',';
    }
    return result;
}