#include "Channel.h"

std::string Channel::getChannelName() {
    return channelName;
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
    return (itr != channelusers.end());
}

std::string listUsers() {
    std::map<std::string, User>::iterator itr;
    std::string list = channelName + " members: ";

    for(itr = channelUsers.begin(); itr != channelUsers.end(); itr++) {
        list += (*itr).getUsername() + " ";
    }

    return list;
}
