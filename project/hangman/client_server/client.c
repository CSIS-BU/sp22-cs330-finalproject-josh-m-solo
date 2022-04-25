/*
* Josh McGrew
* CS330 Final Project
* Client code for single-user hangman game
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//client function
int client(char* server_ip, char* server_port)
{
	//variable declaration
	int sock_fd; //file descriptor
	struct addrinfo hints, * server_info, * p;
	socklen_t sin_size;
	int error;
	int yes = 1;
	char* status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	error = getaddrinfo(server_ip, server_port, &hints, &server_info);
	if (error)
	{
		errx(1, "&s", gai_strerror(error));
	}

	//create socket
	if ((sock_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)) < 0)
	{
		perror("Server: socket");
		exit(-1);
	}

	//connect to server
	if (connect(sock_fd, server_info->ai_addr, server_info->ai_addrlen) < 0)
	{
		perror("Server: connect");
		exit(-1);
	}

	//CLIENT GAME CODE HERE

	//RECEIVE instructions from server
	//SEND game confirmation input to start the new game
	//RECEIVE word data and guess prompt
	//SEND letter guess
	//repeat until guesses run out or word is completed
	//RECEIVE end game message(s)
	//SEND new game input (start new game or quit)
	//new game started or quit occurs accordingly

	//client should be in a loop of (1) SEND user input to server, (2) RECEIVE info from server, and (3) PRINT recieved information
	//loop until game until user quits, then close the socket

	//done
	close(sock_fd);
	return 0;
}

//main function
int main(int argc, char* argv[])
{

	char* server_ip;
	char* server_port;

	//validate 
	if (argc != 3)
	{
		fprintf(stderr, "Client usage: ./client [server IP] [server port]\n");
		exit(EXIT_FAILURE);
	}

	server_ip = argv[1];
	server_port = argv[2];
	return client(server_ip, server_port);
}