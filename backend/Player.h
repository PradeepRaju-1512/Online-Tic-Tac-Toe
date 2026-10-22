#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;
class Player{
private:
    string username;
    string token;
    char symbol;
    bool connected;
public:
    Player();
    Player(string username,string token,char symbol);
    string getUsername() const;
    string getToken() const;
    char getSymbol() const;
    bool isConnected() const;
    void setUsername(string username);
    void setSymbol(char symbol);
    void setConnected(bool connected);
};
#endif