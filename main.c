#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "http.h"

void die(char* msg) {
	printf("Error: %s\n", msg);
	printf("Errno: [%d]: %s\n", errno, strerror(errno));
	exit(0);
}

int create_socket(int port) {
	int sockfd = 0;
	struct sockaddr_in name;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		die("Could not create socket");	

	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	int y = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
		die("Could not set REUSEADDR");

	if(bind(sockfd, (struct sockaddr*)&name, sizeof(name)) == -1)
		die("Could not bind to socket");

	if(listen(sockfd, 511) == -1)
		die("Could not listen on socket");

	return sockfd;
}

int accept_client(int sockfd) {
	int clientfd;
	struct sockaddr_in name;
	socklen_t name_size;

	name_size = sizeof(struct sockaddr_in);
	clientfd = accept(sockfd, (struct sockaddr*)&name, &name_size);
	if(clientfd == -1)
		die("Could not accept client");

	return clientfd;
}

void read_file(char* filename, char** buffer) {
	FILE* filefd;

	filefd = fopen(filename, "r");
	if(filefd == NULL)
		die("Could not open file");

	size_t nread;
	while((nread = fread(*buffer, 1, 1024, filefd)) > 0) {
	}

	fclose(filefd);
}

int main() 
{
	int port = 80;
	int sockfd, clientfd;
	char* buffer = (char*)malloc(1024);
	struct http_request* request = (struct http_request*)malloc(sizeof(request));
	struct http_response* response = (struct http_response*)malloc(sizeof(response));

	printf("Creating socket\n");
	sockfd = create_socket(port);

	printf("Listening for incoming connections\n");

	clientfd = accept_client(sockfd);
	printf("Client connected\n");

	http_read_request(clientfd, &buffer);
	http_parse_request(request, &buffer);
	
	printf("Method: %s\n", request->method);
	printf("Path: %s\n", request->path);
	
	memset(buffer, 0, 1024);
	read_file("responses/base.html", &buffer);
	http_set_response(response, &buffer, "text/html",  strlen(buffer));
	http_respond(clientfd, response);

	close(clientfd);
	close(sockfd);	

	free(buffer);
	free(request);
	free(response);

	return 0;
}
