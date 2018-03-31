#include <map>

class User;

class Channel {
public:
    Channel(){}
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
