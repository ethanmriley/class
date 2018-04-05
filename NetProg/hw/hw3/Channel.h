#pragma once
#include <map>
#include <string>
#include <mutex>
#include "User.h"

class Channel {
public:
    Channel(){}
    Channel(std::string name);

    std::string getChannelName() const;
    int setChannelName(std::string name);

    int addUser(User addedUser);
    int removeUser(std::string username); //TODO use broadcast function here to let the channel know
    bool containsUser(std::string username) const;
    std::string listUsers() const;
    int kickUser(std::string username);

    int broadcast(std::string message) const;

private:
    std::map<std::string, User> channelUsers;
    std::string channelName;
};
