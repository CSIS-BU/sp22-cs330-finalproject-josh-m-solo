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

#define BUFFER_SIZE 1024

//client function
int client(char* server_ip, char* server_port)
{
	//variable declaration
	int sock_fd; //file descriptor
	struct addrinfo hints, * server_info, * p;
	socklen_t sin_size;
	char buffer[BUFFER_SIZE];
	int error;
	int yes = 1;
	char* status;

	//hangman variables
	int playAgainInput = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	error = getaddrinfo(server_ip, server_port, &hints, &server_info);
	if (error)
	{
		//errx(1, "&s", gai_strerror(error));
		perror("Client: connect");
	}

	//create socket
	if ((sock_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)) < 0)
	{
		perror("Client: socket");
		exit(-1);
	}

	//connect to server
	if (connect(sock_fd, server_info->ai_addr, server_info->ai_addrlen) < 0)
	{
		perror("Client: connect");
		exit(-1);
	}
	printf("CLIENT: I am connected.\n");

	//CLIENT GAME CODE HERE

	//RECEIVE instructions from server and print
	char recc[263] = "test.";
	if (recv(sock_fd, recc, sizeof(recc), 0) < 0)
	{
		perror("Client: receive failed (instructions)");
	}
	printf("CLIENT: I received data: %s", recc);
	//enter game loop
	while (playAgainInput == 1)
	{
		break;
		int wordLength = 0;
		int guesses = 0;
		char lettersGuessed[32];
		char wordInProgress[16];
		char word[16];
		char letter; //the guessed letter
		//info variables
		int info1 = 0;
		int info2 = 0;
		int info3 = 0;
		
		//empty the strings
		memset(wordInProgress, 0, sizeof(wordInProgress));
		memset(lettersGuessed, 0, sizeof(lettersGuessed));
		memset(word, 0, sizeof(word));


		//RECEIVE wordlength

		//enter secondary loop (guesses)
		while (1)
		{
			//RECEIVE information:
			//Receiving 'guesses' to display guesses remaining
			//Receiving 'lettersGuessed' for already guessed letters
			//Receiving 'wordInProgress' to display	

			//print the guess prompt
			printf("\n\n***\nWrong Guesses Remaining: %i\n", guesses);
			printf("You have already guessed: %s\n", lettersGuessed);
			printf("Word: %s\n", wordInProgress);
			printf("Guess the next letter: ");
			//get input and store in 'letter'
			//SEND the letter to the server

			//next the client receives information.
			//info1 refers to if the letter has been guessed already.
			//note that info2 and info3 are bypassed if info1 indicates that a letter has been guessed already
			
			//RECEIVE info1 (0 indicates letter that hasn't been guessed, 1 indicates letter has been guessed.)
			if (info1 == 1)
			{
				//this guess is over
				printf("\n%c has been guessed already.", letter);
			}
			else
			{
				//server now processes the guess.
				//next, info2 refers to whether the guess is correct (0) or incorrect (1)
				//RECEIVE info2
				if (info2 == 0)
				{
					printf("\nThat guess is correct!");
				}
				else
				{
					printf("%c is not in the word. You have lost a guess!", letter);
				}

				//info3 has 3 possible values:
				//game is not over (0) (nothing is printed in this case)
				//game is over and player won (1)
				//game is over and player lost (2)
				//RECEIVE info3
				if (info3 == 1)
				{
					printf("\nYou have guessed the word!");
					break;
				}
				else if (info3 == 2)
				{
					printf("\nYou have run out of guesses! You could not guess the word!");
					break;
				}
				//loops back if break does not occur
			}
		}
		//RECEIVE word from server and reveal it (regardless of whether you won or lost)
		printf("\nGame Over! The word was: %s", word);
		printf("\nWould you like to play again? (1: Yes, 0: No)");
		//get input to playAgainInput
		//SEND playAgain response, playAgain also determines if client loops again too
	}

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