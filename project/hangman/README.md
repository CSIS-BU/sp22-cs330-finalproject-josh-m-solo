# Josh McGrew CS330 Final Project
## Single-User Game: Hangman

###Hangman Rules
Hangman is a word guessing game, played by guessing each letter in a word of known length.
As you guess letters correctly, they will appear in their proper places in the word, allowing you to guess more effectively.
Six incorrect guesses are allowed, one for each body part of the hangman.
Win by guessing all of the letters in the word.
You lose if you guess incorrectly six times.

###How to Run
Run Vagrant in the project directory, open SSH, and navigate to the /hangman/client-server direction. The files server.c and client.c are located here.
If it hasn't been done already, compile the programs with the Makefile by using the command "make".
To run the server, use "./server [port]" Example: ./server 54321
To run the client, open a second SSH window in the same directory and use "./client [IP] [port]" Example: ./client localhost 54321
The game will now be playable in the client window.