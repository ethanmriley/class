class Channel;
class User;

#include <map>
#include <string>

class Server {
public:
    Server(){};
    bool channelExists(std::string channelName);
    int setPassword(std::string pass);
    int addChannel(Channel newChannel);
    int addUser(User newUser);
    int removeUser(std::string username);
    bool containsUser(std::string username);

private:
    std::string password;
    std::map<std::string, Channel> serverChannels;
    std::map<std::string, User> serverUsers;
};
