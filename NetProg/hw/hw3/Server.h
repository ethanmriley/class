#include <map>
#include <string>
#include "User.h"
#include "Channel.h"
#pragma once

class Server {
public:
    Server(){};

    int setPassword(std::string pass);
    bool checkPassword(std::string pass);
    
    int addChannel(Channel newChannel);
    int removeChannel(std::string channelName);
    bool containsChannel(std::string channelName);
    Channel* getChannel(std::string channelName);
    
    int addUser(User newUser);
    int removeUser(std::string username);
    bool containsUser(std::string username);
    int removeFromAllChannels(std::string username);
    User* getUser(std::string username);
    
    std::string listChannels();

    //gonna wanna add a u2u messaging function here

private:
    std::string password;
    std::map<std::string, Channel> serverChannels;
    std::map<std::string, User> serverUsers;
};
