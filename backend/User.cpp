#include "User.h"
User::User(){
    id=0;
    username="";
}
User::User(int id,string username){
    this->id=id;
    this->username=username;
}
int User::getId() const{
    return id;
}
string User::getUsername() const{
    return username;
}
void User::setUsername(string username){
    this->username=username;
}