#include "Channel.cpp"

class Server {
public:
    Channel(){};
    bool channelExists(std::string channelName);
    int setPassword(std::string pass);
    int addUser(User newUser);
    int removeUser(std::string username);
    bool containsUser(std::string username);

private:
    std::string password;
    std::map<std::string, Channel> serverChannels;
};
