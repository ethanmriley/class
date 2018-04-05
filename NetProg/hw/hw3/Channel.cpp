#include "Channel.h"

std::mutex mtx_channel;

Channel::Channel(std::string name) {
    mtx_channel.lock();
    channelName = name;
    mtx_channel.unlock();
}

std::string Channel::getChannelName() const{
    return channelName;
}

int Channel::broadcast(std::string message) const {
    std::map<std::string, User>::const_iterator itr;
    for(itr = channelUsers.begin(); itr != channelUsers.end(); itr++) {
        (itr->second).sendMessage(message);
    }
    return 0;
}

int Channel::setChannelName(std::string name) {
    mtx_channel.lock();
    channelName = name;
    mtx_channel.unlock();
    return 0;
}

int Channel::addUser(User addedUser) {
    mtx_channel.lock();
    channelUsers[addedUser.getUsername()] = addedUser;
    mtx_channel.unlock();
    return 0;
}

int Channel::removeUser(std::string username) {
    mtx_channel.lock();
    channelUsers.erase(username);
    mtx_channel.unlock();
    return 0;
}

bool Channel::containsUser(std::string username) const{
    std::map<std::string, User>::const_iterator itr;
    itr = channelUsers.find(username);
    return (itr != channelUsers.end());
}

std::string Channel::listUsers() const{
    std::map<std::string, User>::const_iterator itr;
    std::string result;
    
    result += "There are currently ";
    result += std::to_string(channelUsers.size());
    result += " members.\n";
    result += channelName + " members: ";

    for(itr = channelUsers.begin(); itr != channelUsers.end(); itr++) {
        result += (&(itr->second))->getUsername() + " ";
    }

    result += '\n';

    return result;
}

int Channel::kickUser(std::string username) {
    mtx_channel.lock();
    channelUsers.erase(username);
    mtx_channel.unlock();
    return 0;
}

//TODO add channel broadcast to all of these
