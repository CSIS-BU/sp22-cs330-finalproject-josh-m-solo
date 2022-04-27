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
	FILE* fp;

	//declare hangman-related variables
	const int HANGMAN_WORD_COUNT = 124; //124 words in hangman_words.txt
	const int MAX_GUESSES = 6;
	//int counter = 0;
	const char* fileName = "hangman_words.txt";
	const char* words[HANGMAN_WORD_COUNT - 1];
	//strings to be used in the game
	const char* instructions = "Welcome to Hangman, a word guessing game.\nEach game, a new word will be chosen and you must guess letters in the word one at a time.\nYou lose if you guess incorrectly 6 times.\nYou win if you guess the entire word.\n\n";
	const char* newGamePrompt = "Type [1] to play a new game or [0] to quit.";
	//const char* basicPrompt = "\nGuess the next letter: ";
	int playAgain = 1;
	//seed rng
	srand(time(0));

	//build address data structure
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	//open file (hangman_words.txt) and put into words[] array
	fp = fopen(fileName, "r");
	//fread(words, 16, 1, fp);
	//while (fscanf(fp, "%s", words[counter]) != EOF)
	//{
	//	counter++;
	//}
	//close file
	fclose(fp);


	error = getaddrinfo(NULL, server_port, &hints, &serv_info);
	if (error)
	{
		//errx(1, "%s", gai_strerror(error));
		perror("Server: getaddrinfo");
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

		//SEND instructions and recieve response, write to playAgain bool
		//loop won't run if player doesn't start the game

		while (playAgain == 1)
		{
			//generate random number and assign new word
			int word_number = rand() % HANGMAN_WORD_COUNT;
			const char* word = words[word_number];

			int wordLength = 0;
			int guesses = MAX_GUESSES; //guesses remaining
			char lettersGuessed[32]; //keep track of guessed letters
			char letter; //the guessed letter
			char wordInProgress[16]; //for the in-progress word, it's reset each time the loop runs

			//find length of the word
			for (wordLength = 0; word[wordLength] != '\0'; wordLength++) {}
			//Initial SEND wordLength to client, may not need?

			memset(wordInProgress, 0, sizeof(wordInProgress));
			memset(lettersGuessed, 0, sizeof(lettersGuessed));
			//intitialize wordInProgress to blank lines for the word length
			for (int i = 0; i < wordLength; i++)
			{
				wordInProgress[i] = '-';
			}

			//enter the guesses loop
			while (1)
			{
				//SEND information to client, client will build the prompt from there
				//Sending 'guesses' for guesses remaining
				//Sending 'lettersGuessed' for already guessed letters
				//Sending 'wordInProgress' to be displayed

				//RECEIVE the letter and store it in 'letter'
				//check if letter has already been guessed
				if (strchr(lettersGuessed, letter) != NULL)
				{
					//letter has been guessed already
					//SEND info that the letter has been guessed already (info1 = 1)
					//this guess loop is over
				}
				else
				{
					//SEND info that the letter hasn't been guessed yet (info1 = 0)

					//if time permits, check here if character is valid (lowercase letter only)
					//if it isn't valid, end the guess
					
					//check if letter is in the word
					if (strchr(word, letter) != NULL)
					{
						//letter is in the word
						//SEND info that guess is correct (info2 = 0)
						//build  display word with new letter(s) in it Ex: "_ E _ _ _"
						//iterate over the word:
						//if the guessed letter is there, place the letter in the corresponding spot in 'wordinProgress'

					}
					else
					{
						//letter is not in the word, decrement guesses
						guesses--;
						//SEND info that guess is incorrect (info2 = 1)
					}

					//guess complete, check if guesses have run out
					if (guesses <= 0)
					{
						//SEND info that game is over (lost) (info3 = 2)
						break;
					}
					//also check if word is completed
					//strcmp??
					if (strcmp(wordInProgress, word) == 0) //may not work?
					{
						//word is completed.
						//SEND info that game is over (won) (info3 = 1)
						break;
					}

					//SEND info that game is not over (info3 = 0)
				}

			}
			//current game is over
			//SEND word to client for reveal
			//RECEIVE play again response (1 sets playAgain to true, 0 sets playAgain to false)
			//loop will end if player decides not to play again
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