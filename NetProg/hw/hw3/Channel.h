#include <map>
#include <string>
#include "User.h"

#ifndef _CHANNEL_
#define _CHANNEL_

class Channel {
public:
    Channel(){}
    Channel(std::string name);
    std::string getChannelName();
    int setChannelName(std::string name);
    int addUser(User addedUser);
    int removeUser(std::string username);
    bool containsUser(std::string username);
    std::string listUsers();

private:
    std::map<std::string, User> channelUsers;
    std::string channelName;
};

#endif
