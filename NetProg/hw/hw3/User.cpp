#include "User.h"

User::User() {
    isOperator = false;
}

User::User(std::string user) {
    isOperator = false;
    username = user;
}

std::string User::getUsername() {
    return username;
}

void User::setUsername(std::string user) {
    username = user;
}

void User::setSockfd(int socket) {
    sockfd = socket;
}

void User::setOperator(bool isOp) {
    isOperator = isOp;
}

bool User::userIsOperator() {
    return isOperator;
}
