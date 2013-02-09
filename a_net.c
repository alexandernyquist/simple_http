#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "a_utils.h"

int create_socket(int port) {
	int sockfd = 0;
	struct sockaddr_in name;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		a_die("Could not create socket");	

	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	int y = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1)
		a_die("Could not set REUSEADDR");

	if(bind(sockfd, (struct sockaddr*)&name, sizeof(name)) == -1)
		a_die("Could not bind to socket");

	if(listen(sockfd, 511) == -1)
		a_die("Could not listen on socket");

	return sockfd;
}

int accept_client(int sockfd) {
	int clientfd;
	struct sockaddr_in name;
	socklen_t name_size;

	name_size = sizeof(struct sockaddr_in);
	clientfd = accept(sockfd, (struct sockaddr*)&name, &name_size);
	if(clientfd == -1)
		a_die("Could not accept client");

	return clientfd;
}