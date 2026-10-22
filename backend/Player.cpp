#include "Player.h"
Player::Player(){
    username="Guest";
    token="";
    symbol=' ';
    connected=false;
}
Player::Player(string username,string token,char symbol){
    this->username=username;
    this->token=token;
    this->symbol=symbol;
    connected=true;
}
string Player::getUsername() const{
    return username;
}
string Player::getToken() const{
    return token;
}
char Player::getSymbol() const{
    return symbol;
}
bool Player::isConnected() const{
    return connected;
}
void Player::setUsername(string username){
    this->username=username;
}
void Player::setSymbol(char symbol){
    this->symbol=symbol;
}
void Player::setConnected(bool connected){
    this->connected=connected;
}