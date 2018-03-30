#include "User.h"

void User::setUsername(std::string user) {
    username = user;
}

void User::setSockfd(int socket) {
    sockfd = socket;
}

void User::setOperator(bool isOp) {
    isOperator = isOp;
}
