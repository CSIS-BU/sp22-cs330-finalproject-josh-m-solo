/*
* Josh McGrew
* CS330 Final Project
* Client
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

	//client actions here

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