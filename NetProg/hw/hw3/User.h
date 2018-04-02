#pragma once
#include <string>

class User{
public:
    User();
    User(std::string user_name);

    int sendMessage(std::string message);

    int disconnect();

    void setUsername(std::string username);
    std::string getUsername();
    
    void setSockfd(int sockfd);
    
    void setOperator(bool isOperator);
    bool userIsOperator();


private:
    std::string username;
    bool isOperator;
    int sockfd;
};
