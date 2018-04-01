#include "Server.h"

bool Server::channelExists(std::string channelName) {
    std::map<std::string, Channel>::iterator itr;
    itr = serverChannels.find(channelName);
    return (itr != serverChannels.end());
}

int Server::setPassword(std::string pass) {
    password = pass;
    return 0;
}

bool Server::checkPassword(std::string pass) {
    return pass == password;
}

int Server::addChannel(Channel newChannel) {
    serverChannels[newChannel.getChannelName()] = newChannel;
    return 0;
}

int Server::removeChannel(std::string channel_name) {
    serverChannels.erase(channel_name);
    return 0;
}

int Server::addUser(User newUser) {
    serverUsers[newUser.getUsername()] = newUser;
    return 0;
}

int Server::removeUser(std::string username) {
    serverUsers.erase(username);
    return 0;
}

bool Server::containsUser(std::string username) {
    std::map<std::string, User>::iterator itr;
    itr = serverUsers.find(username);
    return (itr != serverUsers.end());
}

std::string Server::listChannels() {
    std::string result = "";

    std::map<std::string, Channel>::iterator itr;
    for(itr = serverChannels.begin(); itr != serverChannels.end(); itr++) {
        result += "* " + (itr->second).getChannelName() + "\n";
    }

    return result;
}   
