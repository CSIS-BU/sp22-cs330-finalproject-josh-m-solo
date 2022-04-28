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

#define BUFFER_SIZE 2048

void printHangmanImage(int guesses)
{
	char* lOne = "  * * * \n *     *\n *     *\n  * * *";
	char* lTwo = "  * * * \n *     *\n *     *\n  * * *\n    *\n    *\n    *\n    *";
	char* lThree = "  * * * \n *     *\n *     *\n  * * *\n    *\n* * *\n    *\n    *";
	char* lFour = "  * * * \n *     *\n *     *\n  * * *\n    *\n* * * * *\n    *\n    *";
	char* lFive = "  * * * \n *     *\n *     *\n  * * *\n    *\n* * * * *\n    *\n    *\n   *\n  *\n *";
	char* lSix = "  * * * \n * x x *\n * ___ *\n  * * *\n    *\n* * * * *\n    *\n    *\n   * *\n  *   *\n *     *";

	switch (guesses)
	{
	case 5:
		printf("\n%s", lOne);
		break;
	case 4:
		printf("\n%s", lTwo);
		break;
	case 3:
		printf("\n%s", lThree);
		break;
	case 2:
		printf("\n%s", lFour);
		break;
	case 1:
		printf("\n%s", lFive);
		break;
	case 0:
		printf("\n%s", lSix);
		break;
	default:
		printf("\n");
		break;
	}
}

//client function
int client(char* server_ip, char* server_port)
{
	//variable declaration
	int sock_fd;
	struct addrinfo hints, * server_info, * p;
	socklen_t sin_size;
	char buffer[BUFFER_SIZE] = { 0 };
	int error;
	int yes = 1;
	char* status;

	//hangman variables
	int playAgainInput;
	const int MAX_GUESSES = 6;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	error = getaddrinfo(server_ip, server_port, &hints, &server_info);

	//print instructions
	printf("Welcome to Hangman, a word guessing game.\nEach game, a new word will be chosen and you must guess letters in the word one at a time.\nYou lose if you guess incorrectly 6 times.\nYou win if you guess the entire word.\n\nType [1] to play a new game or [0] to quit: ");
	scanf("%d", &playAgainInput);
	while ((getchar()) != '\n');

	while (1)
	{
		if (playAgainInput != 0 && playAgainInput != 1)
		{
			printf("Enter [0] or [1]: ");
			scanf("%d", &playAgainInput);
			while ((getchar()) != '\n');
		}
		else { break; }
	}

	//enter game loop
	while (playAgainInput == 1)
	{
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

		//CLIENT GAME CODE HERE

		//receive the word from the server
		if (recv(sock_fd, buffer, BUFFER_SIZE, 0) < 0)
		{
			perror("Client: receive failed");
		}

		//set word
		const char* word = buffer;

		int wordLength = 0;
		int guesses = MAX_GUESSES;
		char lettersGuessed[32];
		char wordInProgress[16];
		char letter;
		char* image;
		memset(wordInProgress, 0, sizeof(wordInProgress));
		memset(lettersGuessed, 0, sizeof(lettersGuessed));
		//find length of the word
		for (wordLength = 0; word[wordLength] != '\0'; wordLength++) {}

		printf("\nYour new word has been chosen.");

		//intitialize wordInProgress to blank spaces based on word length
		for (int i = 0; i < wordLength; i++)
		{
			wordInProgress[i] = '-';
		}
		//printf("INPUT: %d\nWORD: %s\nLENGTH: %d\nWIP: %s", playAgainInput, word, wordLength, wordInProgress);
		//enter the guesses loop
		while (1)
		{
			//print the guess prompt
			printHangmanImage(guesses);
			printf("\n\n# # #\n\nWrong Guesses Remaining: %i\n", guesses);
			printf("You have already guessed: %s\n", lettersGuessed);
			printf("Word: %s\n", wordInProgress);
			printf("The word has %d letters.\n", wordLength);
			printf("Guess the next letter: ");
			//get next guess
			scanf("%c", &letter);
			while ((getchar()) != '\n');

			//check if letter has already been guessed
			if (strchr(lettersGuessed, letter) == NULL)
			{
				//letter has not been guessed yet
				//check if letter is in the word
				if (strchr(word, letter) != NULL)
				{
					//letter is in the word
					//build display word with new letter(s)
					printf("\nThat guess is correct!\n");
					for (int j = 0; j < wordLength; j++)
					{
						if (word[j] == letter)
						{
							wordInProgress[j] = letter;
						}
					}
				}
				else
				{
					//letter is not in the word, decrement guesses
					printf("\n%c is not in the word. You have lost a guess!\n", letter);
					guesses--;
				}
				//add letter to guessed letters
				strncat(lettersGuessed, &letter, 1);
			}
			else
			{
				//letter has been guessed already
				printf("\n%c has been guessed already.\n", letter);
			}
			//check if guesses have run out
			if (guesses <= 0)
			{
				printf("\nYou have run out of guesses! You could not guess the word!\n");
				printHangmanImage(guesses);
				break;
			}
			//also check if word is completed
			if (strcmp(wordInProgress, word) == 0)
			{
				printf("\nYou have guessed the word!");
				break;
			}
		}
		//game is over, reveal word
		printf("\nGame Over! The word was: %s\n\nWould you like to play again? [1] Yes, [0] No: ", word);
		scanf("%d", &playAgainInput);
		while ((getchar()) != '\n');
		while (1)
		{
			if (playAgainInput != 0 && playAgainInput != 1)
			{
				printf("Enter [0] or [1]: ");
				scanf("%d", &playAgainInput);
				while ((getchar()) != '\n');
			}
			else { break; }
		}
		close(sock_fd);
	}
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