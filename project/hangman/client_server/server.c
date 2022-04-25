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

//server function
int server(char* server_port)
{
	//socket variables declaration
	int sock_fd, new_sock_fd;
	struct addrinfo hints, * serv_info;
	struct sockaddr_in client_addr;
	socklen_t length;
	int error;
	int yes = 1;

	//declare hangman-related variables
	const int HANGMAN_WORD_COUNT = 124; //124 words in hangman_words.txt
	int counter = 0;
	const char* fileName = "hangman_words.txt";
	const char* words[HANGMAN_WORD_COUNT - 1];
	const char* instructions = "Welcome to Hangman, a word guessing game.\nEach game, a new word will be chosen and you must guess letters in the word one at a time.\nYou lose if you guess incorrectly 6 times.\nYou win if you guess the entire word.";
	//seed rng
	srand(time(0));

	//build address data structure
	memset(&hints, 0, sizeof hints);
	hint.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_FASSIVE;

	//open file (hangman_words.txt) and put into words[] array
	if (FILE* fp = fopen(fileName, "r") == NULL)
	{
		perror("File: could not open file");
		exit(0);
	}
	while (fscanf(fp, "%s", &words[counter]) != EOF)
	{
		counter++;
	}
	//close file
	fclose(fp);


	error = getaddrinfo(NULL, server_port, &hints, &serv_info);
	if (error)
	{
		errx(1, "%s", gai_strerror(error));
	}

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

		//GAME CODE HERE

		//note, needs to be in a loop in the future so user can play multiple games

		//generate random number and assign word
		int word_number = rand() % HANGMAN_WORD_COUNT;
		const char word = words[word_number];

		//SEND instructions to client
		//RECEIVE client's confirmation input to start the new game
		//SEND word data and guess prompt to client (in loop) until wrong guesses run out or word is completed
		//RECEIVE client's guessed letter
		//process letter and update word data
		//SEND end of game message(s) and new game prompt
		//RECEIVE client's new game input
		//start new game or terminate accordingly


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