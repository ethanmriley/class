#include "User.h"
#include <arpa/inet.h>
#include <sys/socket.h>

std::mutex mtx;

User::User() {
    isOperator = false;
}

User::User(std::string user) {
    isOperator = false;
    username = user;
}

int User::disconnect() {
    return 0;
}

int User::sendMessage(std::string message) {
    int result;
    result = send(sockfd, message.c_str(), message.length(), 0);
    return 0;
}

std::string User::getUsername() {
    return username;
}

void User::setUsername(std::string user) {
    mtx.lock();
    username = user;
    mtx.unlock();
}

void User::setSockfd(int socket) {
    mtx.lock();
    sockfd = socket;
    mtx.unlock();
}

void User::setOperator(bool isOp) {
    mtx.lock();
    isOperator = isOp;
    mtx.unlock();
}

bool User::userIsOperator() {
    return isOperator;
}
