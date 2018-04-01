#include <string>

#ifndef _USER_
#define _USER_

class User{
public:
    User();
    User(std::string user_name);

    void setUsername(std::string username);
    std::string getUsername();
    
    void setSockfd(int sockfd);
    
    void setOperator(bool isOperator);
    bool userIsOperator();

    int sendMessage(std::string message);

private:
    std::string username;
    bool isOperator;
    int sockfd;
};

#endif
