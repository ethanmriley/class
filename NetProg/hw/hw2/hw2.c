#include <dns_sd.h>
#include <sys/time.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <unistd.h>

//zeroconf rps server
//ethan riley, 2018

//this struct holds the state for the game
struct game
    {
    char player_names[2][40];
    char player_choices[2][10];
    int  player_sockets[2];
    //keep track of which player we're talking to
    int  player_number;  
    };

//helper struct to make getting a socket a little easier
struct server_socket 
    {
    int sockfd;
    struct sockaddr_in sockaddr;
    };

//convert a string in place to uppercase
char* strupper(char* input_string)
    {
    for(unsigned int i = 0; i < strlen(input_string); i++)
        input_string[i] = toupper((int)input_string[i]);
    }

//checks for whitespace, removes newlines
int validateName(char* string)
    {
    int result = 0;
    for(unsigned int i = 0; i < strlen(string); i++)
        {
        //if we've encountered a newline, null it 
        if(string[i] == '\n')
            {
            string[i] = '\0';
            continue;
            }

        //flip the result if we encounter any non-whitespace/null characters
        if((isspace(string[i]) == 0) && ((int)string[i] != 0))
            result = 1;
        }
    return result;
    }

struct server_socket get_socket()
    {
    struct server_socket rps_sock;
    unsigned int sockaddr_len = sizeof(rps_sock.sockaddr);
    if ((rps_sock.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // ask for an ipv4 TCP socket 
        {
        perror("get_socket failed");
        exit(EXIT_FAILURE);
        }
    
    bzero(&rps_sock.sockaddr, sockaddr_len);

    rps_sock.sockaddr.sin_family      = AF_INET;
    rps_sock.sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    rps_sock.sockaddr.sin_port        = 0; //ask for a port from the OS
    
    if (bind(rps_sock.sockfd, (struct sockaddr*)&rps_sock.sockaddr, sockaddr_len) == -1)
        {
        perror("bind failed");
        exit(EXIT_FAILURE);
        }

    //fill in rps_sock.sockaddr.sin_port with the port the OS assigned us
    if (getsockname(rps_sock.sockfd, (struct sockaddr*)&rps_sock.sockaddr, &sockaddr_len) == -1)
        {
        perror("getsockname failed");
        exit(EXIT_FAILURE);
        }

    return rps_sock;
    }

//from zeroconf code exactly
static void
MyRegisterCallBack(DNSServiceRef service,
				   DNSServiceFlags flags,
				   DNSServiceErrorType errorCode,
				   const char * name,
				   const char * type,
				   const char * domain,
				   void * context)
	{
	#pragma unused(flags)
	#pragma unused(context)

	if (errorCode != kDNSServiceErr_NoError)
		fprintf(stderr, "MyRegisterCallBack returned %d\n", errorCode);
	else
		printf("%-15s %s.%s%s\n","REGISTER", name, type, domain);
	}

DNSServiceRef advertiseService(short port) 
    {
    DNSServiceErrorType error;
    DNSServiceRef serviceRef;
	error = DNSServiceRegister(&serviceRef,
								0,                  // no flags
								0,                  // all network interfaces
								"rileye4",          // name
								"_rps._tcp",        // service type
								"",                 // register in default domain(s)
								NULL,               // use default host name
								port,               // port number
								0,                  // length of TXT record
								NULL,               // no TXT record
								MyRegisterCallBack, // call back function
								NULL);              // no context
    if (error != 0) 
        {
        perror("DNSServiceRegister failed");
        exit(EXIT_FAILURE);
        }
    return serviceRef;
    }

//undo the effects of the most recent connection
void undo_game(struct game* current_game)
    {
    int current_player = (*current_game).player_number;
    bzero((*current_game).player_names[current_player], 40);
    bzero((*current_game).player_choices[current_player], 10);
    (*current_game).player_sockets[current_player] = 0;
    (*current_game).player_number--;
    return;
    }

//reset state to the beginning of a game
void reset_game(struct game* current_game)
    {
    bzero(current_game, sizeof(struct game));
    (*current_game).player_number = -1;
    }

void new_connection(int servfd, struct game* current_game)
    {
    struct sockaddr_in client;
    int cliaddr_len = sizeof(client);
    int err;
    int bytes_recv;
    int new_sock;
    int current_player;
    char ask_name[] = "What is your name?\n";
    char ask_rps[] = "Rock, paper, or scissors?\n";
    char name[40] = {0};
    char choice[10] = {0};

    new_sock = accept(servfd, (struct sockaddr*)&client, &cliaddr_len);
    if (new_sock == -1)
        {
        perror("accept failed");
        return;
        }

    (*current_game).player_number++; //we're handling a new player, so increment counter
    current_player = (*current_game).player_number;
    (*current_game).player_sockets[current_player] = new_sock; //keep track of the sockfd

    while(validateName(name) == 0)
        {
        err = send(new_sock, &ask_name, sizeof(ask_name), 0);
        if (err == -1) //if there was an error in sending (the socket closed) undo the game
            {
            undo_game(current_game);
            return;
            }

        bytes_recv = recv(new_sock, &name, sizeof(name), 0);
        if (bytes_recv == 0) //if we only recv 0 bytes then the connection closed 
            {
            undo_game(current_game);
            return;
            }
        }

    while((strcmp(choice, "ROCK") != 0) && (strcmp(choice, "PAPER") != 0) && (strcmp(choice, "SCISSORS") != 0))
        {
        err = send(new_sock, &ask_rps, sizeof(ask_rps), 0);
        if (err == -1)
            {
            undo_game(current_game);
            return;
            }

        bytes_recv = recv(new_sock, &choice, sizeof(choice), 0);
        if (bytes_recv == 0)
            {
            undo_game(current_game);
            return;
            }
        strupper(choice);
        choice[strlen(choice) - 1] = '\0';
        }


    //fill in our game struct with the current player's info
    memcpy((*current_game).player_names[current_player], &name, sizeof(name));
    memcpy((*current_game).player_choices[current_player], &choice, sizeof(choice));

    return;
    }

//takes a filled in game struct and determines the winner, sends that info to both clients, and closes sockets
void finish_game(struct game* current_game)
    {
    char player1[40];
    char choice1[10];
    char player2[40];
    char choice2[10];
    int  socket1;
    int  socket2;
    char result[200] = {0};
    char test[] = "test\n";
    int  winner;

    memcpy(&player1, (*current_game).player_names[0], sizeof(player1));
    memcpy(&choice1, (*current_game).player_choices[0], sizeof(choice1));
    memcpy(&player2, (*current_game).player_names[1], sizeof(player2));
    memcpy(&choice2, (*current_game).player_choices[1], sizeof(choice2));
    socket1 = (*current_game).player_sockets[0];
    socket2 = (*current_game).player_sockets[1];


    if (strcmp(choice1, "ROCK") == 0)    
        {
        if (strcmp(choice2, "ROCK") == 0)    
            sprintf(result, "ROCK ties ROCK! %s ties %s!\n", player1, player2);
        else if (strcmp(choice2, "PAPER") == 0)    
            sprintf(result, "PAPER covers ROCK! %s defeats %s!\n", player2, player1);
        else if (strcmp(choice2, "SCISSORS") == 0)    
            sprintf(result, "ROCK smashes SCISSORS! %s defeats %s!\n", player1, player2);
        }
    else if (strcmp(choice1, "PAPER") == 0)    
        {
        if (strcmp(choice2, "ROCK") == 0)    
            sprintf(result, "PAPER covers ROCK! %s defeats %s!\n", player1, player2);
        else if (strcmp(choice2, "PAPER") == 0)    
            sprintf(result, "PAPER ties PAPER! %s ties %s!\n", player1, player2);
        else if (strcmp(choice2, "SCISSORS") == 0)    
            sprintf(result, "SCISSORS cuts PAPER! %s defeats %s!\n", player2, player1);
        }
    else if (strcmp(choice1, "SCISSORS") == 0)    
        {
        if (strcmp(choice2, "ROCK") == 0)    
            sprintf(result, "ROCK smashes SCISSORS! %s defeats %s!\n", player2, player1);
        else if (strcmp(choice2, "PAPER") == 0)    
            sprintf(result, "SCISSORS cuts PAPER! %s defeats %s!\n", player1, player2);
        else if (strcmp(choice2, "SCISSORS") == 0)    
            sprintf(result, "SCISSORS ties SCISSORS! %s ties %s!\n", player1, player2);
        }

    //we don't care if the info makes it so no error checking on these
    send(socket1, &result, strlen(result), 0);
    send(socket2, &result, strlen(result), 0);

    close(socket1);
    close(socket2);

    reset_game(current_game);

    return;
    
    }

int main (int argc, const char * argv[])
	{
    int daemonfd;
    int nfds;
    int result;
    struct timeval tv;
    struct server_socket rps_sock;
    struct game current_game = {0};
    DNSServiceRef serviceRef;
    fd_set readfds;

    //prevent process from stopping when a send() fails
    signal(SIGPIPE, SIG_IGN);
    
    //get a socket and advertise our service
    rps_sock = get_socket(); 
    serviceRef = advertiseService(rps_sock.sockaddr.sin_port);

    daemonfd = DNSServiceRefSockFD(serviceRef);
    nfds = daemonfd + 1;

    //initialize player number
    current_game.player_number = -1;

    while(1) 
        {
        FD_ZERO(&readfds);
        FD_SET(daemonfd, &readfds);
        FD_SET(rps_sock.sockfd, &readfds);
        tv.tv_sec = 1000000;
        tv.tv_usec = 0;

        listen(rps_sock.sockfd, 20);

        result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
        if (result > 0) 
            {
            if (FD_ISSET(daemonfd, &readfds))
                DNSServiceProcessResult(serviceRef);
            
            if (FD_ISSET(rps_sock.sockfd, &readfds))
                new_connection(rps_sock.sockfd, &current_game);
            }
        else 
            perror("select failed");
        
        //check the player number and finish the game
        if (current_game.player_number == 1)
            finish_game(&current_game);
       
        }

	return 0;
	}
