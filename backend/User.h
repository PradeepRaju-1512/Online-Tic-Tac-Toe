#ifndef USER_H
#define USER_H
#include <string>
using namespace std;
class User{
private:
    int id;
    string username;
public:
    User();
    User(int id,string username);
    int getId() const;
    string getUsername() const;
    void setUsername(string username);
};
#endif