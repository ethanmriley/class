#include <string>

class User{
public:
    User(){};
    void setUsername(std::string username);
    void setSockfd(int sockfd);
    void setOperator(bool isOperator);
    bool userIsOperator();
    std::string getUsername();

private:
    std::string username;
    bool isOperator;
    int sockfd;
};
