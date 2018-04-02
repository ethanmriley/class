#include "Server.h"
#include <assert.h>
#include <iostream>
#include <regex>

using std::cout;
using std::string;
using std::map;

void userTests() {
    User user;

    assert(user.getUsername() == "");

    user.setUsername("Maria");

    assert(user.getUsername() == "Maria");

    assert(user.userIsOperator() == false);

    user.setOperator(true);

    assert(user.userIsOperator() == true);
}

void channelTests() {
    Channel channel;

    assert(channel.getChannelName() == "");

    channel.setChannelName("#news");

    assert(channel.getChannelName() == "#news");

    assert(channel.containsUser("Maria") == false);

    User Maria = User("Maria");

    channel.addUser(Maria);

    assert(channel.containsUser("Maria") == true);

    User pwnz0r = User("pwnz0r");
    channel.addUser(pwnz0r);
    User john = User("john");
    channel.addUser(john);

    assert(channel.listUsers() == "#news members: Maria john pwnz0r \n");

    assert(channel.containsUser("pwnz0r") == true);

    channel.removeUser("pwnz0r");

    assert(channel.containsUser("pwnz0r") == false);
}

void serverTests() {
    Server serv;

    serv.setPassword("secret");

    assert(serv.checkPassword("password") == false);

    assert(serv.checkPassword("secret") == true);

    serv.addChannel(Channel("#news"));

    assert(serv.channelExists("#news") == true);

    serv.removeChannel("#news");

    assert(serv.channelExists("#news") == false);

    serv.addUser(User("Maria"));

    assert(serv.containsUser("Maria") == true);

    serv.removeUser("Maria");

    assert(serv.containsUser("Maria") == false);

    serv.addChannel(Channel("#news"));
    serv.addChannel(Channel("#trivia"));
    serv.addChannel(Channel("#announcements"));

    assert(serv.listChannels() == "* #announcements\n* #news\n* #trivia\n");
}

bool validChannelName(string channelName) {
    return (std::regex_match(channelName, std::regex("#[a-zA-Z][_0-9a-zA-Z]*")) && channelName.length() <= 20);
}

bool validUsername(string username) {
    return (std::regex_match(username, std::regex("[a-zA-Z][_0-9a-zA-Z]*")) && username.length() <= 20);
}

string USER(string username, Server &serv) {
    if(validUsername(username)) {
        serv.addUser(User(username));
        return "Welcome, " + username + "\n";
    } else {
        return "Invalid username.\n";
    }
}


string LIST(string channelName, Server &serv) {
    if(serv.channelExists(channelName)) {
        return serv.getChannel(channelName)->listUsers();
    }
    else {
        return serv.listChannels();
    }
}

string JOIN(User user, string channelName, Server &serv) {
    if(serv.channelExists(channelName)) {
        serv.getChannel(channelName)->addUser(user); //currentUser should already be validated
    } else {
        //we have to make the channel
        //validate channelName, create the channel, add our user to channel, add channel to serv
        if(validChannelName(channelName)) {
            Channel newChannel = Channel(channelName);
            newChannel.addUser(user);
            serv.addChannel(newChannel);
        } else {
            return "Invalid channel name.\n";
        }

    }
    
    return ("Joined channel " + channelName + "\n");
}

string PART(string currentUser, string channelName, Server &serv) {
    if(channelName != "all") {
        if(serv.getChannel(channelName)->containsUser(currentUser)) {
            serv.getChannel(channelName)->removeUser(currentUser);
            return "Left channel " + channelName + "\n";
        } else {
            return "You are not currently in " + channelName + "\n";
        }
    } else {
        serv.removeFromAllChannels(currentUser);
    }
}

//TODO make channels' user dictionaries point to the server's to save memory 

string OPERATOR(string password, string currentUser, Server &serv) {
    if(serv.checkPassword(password)) {
        serv.getUser(currentUser)->setOperator(true);
        return "OPERATOR status bestowed.\n";
    } else {
        return "Invalid OPERATOR command\n";
    }
}

string KICK(string currentUser, string kickedUser, string channelName, Server &serv) {
    if(serv.getUser(currentUser)->userIsOperator()) {
        if(serv.getChannel(channelName)->containsUser(kickedUser)) {
            serv.getChannel(channelName)->kickUser(kickedUser);
        }
    }
    
    return "";
}

string PRIVMSG(string currentUser, string recipient, string message, Server &serv) {
    if(message.length() > 512) {
        return "Message too long.";
    }

    if(recipient[0] == '#') {
        if(serv.channelExists(recipient)) {
            serv.getChannel(recipient)->broadcast(message);
            return "";
        } else {
            return "Channel not found.";
        }
    } else {
        if(serv.containsUser(recipient)) {
            serv.getUser(recipient)->sendMessage(message);
        } else {
            return "User not found.";
        }
    }
}

string QUIT(string currentUser, Server &serv) {
    serv.removeFromAllChannels(currentUser);
    serv.getUser(currentUser)->disconnect();
}

void USERTests() {
    Server serv;

    assert(USER("Maria", serv) == "Welcome, Maria\n");

    assert(serv.containsUser("Maria") == true);

    assert(USER("pwnZ0r", serv) == "Welcome, pwnZ0r\n");

    assert(serv.containsUser("pwnZ0r") == true);

    assert(USER("ethan riley", serv) == "Invalid username.\n");

    assert(serv.containsUser("ethan riley") == false);
}

void LISTTests() {
    Server serv;

    serv.addChannel(Channel("#news"));
    serv.addChannel(Channel("#trivia"));
    serv.addChannel(Channel("#support"));

    serv.getChannel("#news")->addUser(User("Maria"));
    serv.getChannel("#news")->addUser(User("pwnZ0r"));
    serv.getChannel("#news")->addUser(User("ethan"));

    assert(LIST("", serv) == "* #news\n* #support\n* #trivia\n"); //should document this
    assert(LIST("#news", serv) == "#news members: Maria ethan pwnZ0r \n");
}

int main(int argc, char** argv) {
    userTests();

    channelTests();

    serverTests();

    USERTests();

    LISTTests();

    //JOINTests();

    //PARTTests();

    //OPERATORTests();

    //KICKTests();

    //PRIVMSGTests();

    //QUITTests();

    return 0;
}
