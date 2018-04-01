#include <map>
#include <string>
#include "User.h"
#include "Channel.h"

class Server {
public:
    Server(){};

    int setPassword(std::string pass);
    bool checkPassword(std::string pass);
    
    int addChannel(Channel newChannel);
    int removeChannel(std::string channelName);
    bool channelExists(std::string channelName);
    Channel* getChannel(std::string channelName);
    
    int addUser(User newUser);
    int removeUser(std::string username);
    bool containsUser(std::string username);
    
    std::string listChannels();

private:
    std::string password;
    std::map<std::string, Channel> serverChannels;
    std::map<std::string, User> serverUsers;
};
