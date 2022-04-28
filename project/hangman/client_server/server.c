/*
* Josh McGrew
* CS330 Final Project
* Server code for single-user hangman game
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define QUEUE_LENGTH 10
#define BUFFER_SIZE 2048

//server function
int server(char* server_port)
{
	//socket variables declaration
	//char buffer[BUFFER_SIZE] = { 0 };
	int sock_fd, new_sock_fd;
	struct addrinfo hints, * serv_info;
	struct sockaddr_in client_addr;
	socklen_t length;
	int error;
	int yes = 1;
	int buf_len;

	//declare hangman related variables
	const int HANGMAN_WORD_COUNT = 34;
	const char* words[] = { "birthday", "camera", "challenge", "creation", "database", "emotion", "energy", "foundation", "generator", "hardware", "homework", "inspector", "jacket", "kingdom", "library", "manager", "morning", "nightmare", "office", "packet", "payment", "protocol", "resources", "router", "science", "session", "software", "switch", "technology", "thunder", "vehicle", "winter", "yogurt" };
	
	srand(time(0));

	//build address data structure
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	error = getaddrinfo(NULL, server_port, &hints, &serv_info);

	//create socket
	if ((sock_fd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol)) < 0)
	{
		perror("Server: socket");
		exit(-1);
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(-1);
	}

	//bind
	if (bind(sock_fd, serv_info->ai_addr, serv_info->ai_addrlen) < 0)
	{
		close(sock_fd);
		perror("Server: bind error");
		exit(-1);
	}
	freeaddrinfo(serv_info);

	if (serv_info == NULL)
	{
		fprintf(stderr, "Server: failed to bind\n");
		exit(-1);
	}

	//prepare for clients
	if (listen(sock_fd, QUEUE_LENGTH) < 0)
	{
		perror("Server: listen error");
		exit(-1);
	}

	//wait for connection
	while (1)
	{
		length = sizeof client_addr;
		if ((new_sock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &length)) < 0)
		{
			perror("Server: accept error");
			exit(-1);
		}

		//Game code
		//generate random number and assign word
		int word_number = rand() % HANGMAN_WORD_COUNT;
		const char* word = words[word_number];
		
		//SEND the word to the client
		if (send(new_sock_fd, word, strlen(word), 0) < 0)
		{
			perror("Server: send failed");
		}

		//done
		close(new_sock_fd);
	}
	return 0;
}

//main function
int main(int argc, char* argv[])
{
	char* server_port;

	if (argc != 2)
	{
		fprintf(stderr, "Server usage: ./server [server port]\n");
		exit(EXIT_FAILURE);
	}

	server_port = argv[1];
	return server(server_port);
}