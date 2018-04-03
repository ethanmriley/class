#include "User.h"
#include "Channel.h"
#include "Server.h"
#include "commands.h"
#include "test.h"
#include "network.h"
#include <signal.h>
#include <iostream>
#include <vector>
#include <regex>

using std::cout;
using std::string;
using std::regex_match;
using std::regex;

bool validChannelName(string channelName) {
    return (regex_match(channelName, regex("#[a-zA-Z][_0-9a-zA-Z]*")) && channelName.length() <= 20);
}

bool validUsername(string username) {
    return (regex_match(username, regex("[a-zA-Z][_0-9a-zA-Z]*")) && username.length() <= 20);
}

string parse(string currentUser, char* request, Server &serv) {
    char *token;
    std::vector<char *> tokens;

    token = strtok(request, " ");
    while(token) {
        tokens.push_back(token);
        token = strtok(NULL, " ");
    }

    if(tokens[0] == "USER") { 
        if(tokens.size() != 2)
            return "Invalid USER command.\n";

        return USER(tokens[1], serv);

    } else if(tokens[0] == "LIST") {
        if(tokens.size() > 2)
            return "Invalid LIST command.\n";
        else if(tokens.size() == 1)
            return LIST("", serv);
        else if(tokens.size() == 2)
            return LIST(tokens[1], serv);

    } else if(tokens[0] == "JOIN") {
        if(tokens.size() != 2) 
            return "Invalid JOIN command.\n";
        
        return JOIN(currentUser, tokens[1], serv);

    } else if(tokens[0] == "PART") { 
        if(tokens.size() > 2)
            return "Invalid PART command.\n";
        else if(tokens.size() == 1)
            return "";
        else if(tokens.size() == 2)
            return "";

    } else if(tokens[0] == "OPERATOR") {
        if(tokens.size() != 2)
            return "Invalid OPERATOR command.\n";
 
    } else if(tokens[0] == "KICK") {
        if(tokens.size() != 3)
            return "Invalid KICK command.\n";
        
    } else if(tokens[0] == "PRIVMSG") {
        if(tokens.size() != 3)
            return "Invalid PRIVMSG command.\n";
        
    } else if(tokens[0] == "QUIT") {
        if(tokens.size() != 1)
            return "Invalid QUIT command.\n";
        
    } else return "Invalid command.\n";
}

int new_connection(int servfd, Server& serv) {
    struct sockaddr_in client;
    unsigned int cliaddr_len = sizeof(client);
    int err;
    int bytes_recv;
    int client_sock;
    char request[1024] = {0};
    string response;
    string currentUser = "placeholder";

    client_sock = Accept(servfd, (struct sockaddr*) &client, &cliaddr_len);

    while(1) {
        bytes_recv = Recv(client_sock, request, sizeof(request), 0);
        if(bytes_recv == 0)
            break;

        response = parse(currentUser, request, serv);
        Send(client_sock, response.c_str(), response.length(), 0);
    }

    return 0;
}

int main (int argc, const char * argv[]) {
    int nfds;
    int result;
    Server serv;
    struct timeval tv;
    struct server_socket serv_sock;
    fd_set readfds;

    //prevent process from stopping when a send() fails
    signal(SIGPIPE, SIG_IGN);
    
    serv_sock = get_socket(); 
    nfds = serv_sock.sockfd + 1;

    cout << ntohs(serv_sock.sockaddr.sin_port) << '\n';

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(serv_sock.sockfd, &readfds);
        tv.tv_sec = 1000000;
        tv.tv_usec = 0;

        listen(serv_sock.sockfd, 20);

        result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
        if (result > 0) {
            if (FD_ISSET(serv_sock.sockfd, &readfds))
                new_connection(serv_sock.sockfd, serv);
            }
        else 
            perror("select failed");
    }

	return 0;
}
