#include "User.h"
#include <arpa/inet.h>
#include <sys/socket.h>

std::mutex mtx_user;

User::User() {
    isOperator = false;
}

User::User(std::string user, int client_sock) {
    isOperator = false;
    username = user;
    sockfd = client_sock;
}

int User::disconnect() {
    return 0;
}

int User::sendMessage(std::string message) const{
    int result;
    result = send(sockfd, message.c_str(), message.length(), 0);
    return 0;
}

std::string User::getUsername() const{
    return username;
}

void User::setUsername(std::string user) {
    mtx_user.lock();
    username = user;
    mtx_user.unlock();
}

void User::setSockfd(int socket) {
    mtx_user.lock();
    sockfd = socket;
    mtx_user.unlock();
}

void User::setOperator(bool isOp) {
    mtx_user.lock();
    isOperator = isOp;
    mtx_user.unlock();
}

bool User::userIsOperator() const{
    return isOperator;
}
