#pragma once
#include <string>
#include <mutex>

class User{
public:
    User();
    User(std::string user_name, int client_sock);

    int sendMessage(std::string message) const;

    int disconnect();

    void setUsername(std::string username);
    std::string getUsername() const;
    
    void setSockfd(int sockfd);
    
    void setOperator(bool isOperator);
    bool userIsOperator() const;


private:
    std::string username;
    bool isOperator;
    int sockfd;
};
