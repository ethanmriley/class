#include "User.h"
#include "Channel.h"
#include "Server.h"
#include "commands.h"
#include "network.h"
#include <signal.h>
#include <iostream>
#include <vector>
#include <regex>
#include <unistd.h>
#include <thread>

using std::cout;
using std::string;
using std::regex_match;
using std::regex;
using std::thread;

bool validChannelName(string channelName) {
    return (regex_match(channelName, regex("#[a-zA-Z][_0-9a-zA-Z]*")) && channelName.length() <= 20);
}

bool validUsername(string username) {
    return (regex_match(username, regex("[a-zA-Z][_0-9a-zA-Z]*")) && username.length() <= 20);
}

bool validPassword(string password) {
    return (regex_match(password, regex("[a-zA-Z][_0-9a-zA-Z]*")) && password.length() <= 20);
}

string parse(string &currentUser, char* request, Server &serv, bool first_command, int client_sock) {
    char* token;
    std::vector<string> tokens;

    token = strtok(request, " ");
    while(token) {
        
        for(int i = 0; i < sizeof(token); i++) {
            if(token[i] == '\n')
                token[i] = '\0';
        }

        tokens.push_back(string(token));
        token = strtok(NULL, " ");
    }
    
    if(first_command && tokens[0].compare("USER") != 0)
        return "Invalid command, please identify yourself with USER.\n";

    if(tokens[0].compare("USER") == 0) { 
        if(tokens.size() != 2)
            return "Invalid USER command.\n";
        else if(not validUsername(tokens[1]) )
            return "Invalid username.\n";

        currentUser = tokens[1];
        return USER(tokens[1], client_sock, serv);

    } else if(tokens[0].compare("LIST") == 0) {
        if(tokens.size() > 2)
            return "Invalid LIST command.\n";
        else if(tokens.size() == 1)
            return LIST("", serv);
        else if(tokens.size() == 2)
            if(not validChannelName(tokens[1]))
                return "Invalid channel name.\n";

        return LIST(tokens[1], serv);

    } else if(tokens[0].compare("JOIN") == 0) {
        if(tokens.size() != 2) 
            return "Invalid JOIN command.\n";
        else if(not validChannelName(tokens[1]))
            return "Invalid channel name.\n";
        
        return JOIN(currentUser, client_sock, tokens[1], serv);

    } else if(tokens[0].compare("PART") == 0){ 
        if(tokens.size() > 2)
            return "Invalid PART command.\n";
        else if(tokens.size() == 1)
            return PART(currentUser, "", serv);
        else if(tokens.size() == 2)
            if(not validChannelName(tokens[1]))
                return "Invalid channel name.\n";

        return PART(currentUser, tokens[1], serv);

    } else if(tokens[0].compare("OPERATOR") == 0) {
        if(tokens.size() != 2)
            return "Invalid OPERATOR command.\n";
        else if(not validPassword(tokens[1]))
            return "Invalid password.\n";

        return OPERATOR(currentUser, tokens[1], serv);
 
    } else if(tokens[0].compare("KICK") == 0) {
        if(tokens.size() != 3)
            return "Invalid KICK command.\n";
        else if(not validChannelName(tokens[1]))
            return "Invalid channel name.\n";
        else if(not validUsername(tokens[2]))
            return "Invlaid username.\n";

        return KICK(currentUser, tokens[1], tokens[2], serv);
        
    } else if(tokens[0].compare("PRIVMSG") == 0) {
        if(tokens.size() < 3)
            return "Invalid PRIVMSG command.\n";
        if(not (validUsername(tokens[1]) || validChannelName(tokens[1])))
            return "Invalid recipient.\n";
        
        string message;
        for(unsigned int i = 2; i < tokens.size(); i++) {
            message += tokens[i];
            message += " ";
        }

        return PRIVMSG(currentUser, tokens[1], message, serv); //TODO parse messages as one thing rn the spaces will mess it up
        
    } else if(tokens[0].compare("QUIT") == 0) {
        if(tokens.size() != 1)
            return "Invalid QUIT command.\n";

        return QUIT(currentUser, serv);
        
    } else return "Invalid command.\n";
}

int new_connection(int servfd, Server& serv) {
    struct sockaddr_in client;
    unsigned int cliaddr_len = sizeof(client);
    int err;
    int bytes_recv;
    int client_sock;
    bool first_command = true;
    char request[1024] = {0};
    string response;
    string currentUser = "placeholder";

    client_sock = Accept(servfd, (struct sockaddr*) &client, &cliaddr_len);

    while(1) {
        bytes_recv = Recv(client_sock, request, sizeof(request), 0);
        if(bytes_recv == 0)
            break;

        response = parse(currentUser, request, serv, first_command, client_sock);
        Send(client_sock, response.c_str(), response.length(), 0);
        if(first_command) {
            if(response.compare("Invalid command, please identify yourself with USER.\n") == 0) {
                close(client_sock);
                break;
            } else
                first_command = false;
        }
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
                thread (new_connection, serv_sock.sockfd, std::ref(serv)).detach();
            }
        else 
            perror("select failed");
    }

	return 0;
}
