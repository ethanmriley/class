#include "Channel.h"

Channel::Channel(std::string name) {
    channelName = name;
}

std::string Channel::getChannelName() {
    return channelName;
}

int Channel::broadcast(std::string message) {
    std::map<std::string, User>::iterator itr;
    for(itr = channelUsers.begin(); itr != channelUsers.end(); itr++) {
        (itr->second).sendMessage(message);
    }
    return 0;
}

int Channel::setChannelName(std::string name) {
    //might implement checking for name validity here
    channelName = name;
    return 0;
}

int Channel::addUser(User addedUser) {
     channelUsers[addedUser.getUsername()] = addedUser;
     return 0;
}

int Channel::removeUser(std::string username) {
    channelUsers.erase(username);
    return 0;
}

bool Channel::containsUser(std::string username) {
    std::map<std::string, User>::iterator itr;
    itr = channelUsers.find(username);
    return (itr != channelUsers.end());
}

std::string Channel::listUsers() {
    std::map<std::string, User>::iterator itr;
    std::string list = channelName + " members: ";

    for(itr = channelUsers.begin(); itr != channelUsers.end(); itr++) {
        list += (itr->second).getUsername() + " ";
    }

    list += '\n';

    return list;
}

int Channel::kickUser(std::string username) {
    channelUsers.erase(username);
}

//TODO add channel broadcast to all of these
