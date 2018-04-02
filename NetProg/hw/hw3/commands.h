#pragma once

#include "Server.h"
#include <assert.h>
#include <regex>

std::string USER(std::string username, Server &serv);
std::string LIST(std::string channelName, Server &serv);
std::string JOIN(std::string currentUser, std::string channelName, Server &serv);
std::string PART(std::string currentUser, std::string channelName, Server &serv);
std::string OPERATOR(std::string currentUser, std::string passowrd, Server &serv);
std::string KICK(std::string currentUser, std::string kickedUser, std::string channelName, Server &serv);
std::string PRIVMSG(std::string currentUser, std::string recipient, std::string message, Server &serv);
std::string QUIT(std::string currentUser, Server &serv);
bool validChannelName(std::string channelName);
bool validUsername(std::string username);
