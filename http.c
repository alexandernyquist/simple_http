#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "http.h"

void http_read_request(int clientfd, char** buffer) {
	recv(clientfd, *buffer, 1024, 0);
}

void http_parse_request(struct http_request* request, char** buffer) {
	request->method = (char*)malloc(1024);
	request->path = (char*)malloc(1024);

	char rest[255];
	sscanf(*buffer, "%s %s %s", request->method, request->path, rest);
}

void http_set_response(struct http_response* response, char** data, char* content_type, int content_length) {
	response->content_type = (char*)malloc(1024);
	response->data = (char*)malloc(1024);

	response->content_length = content_length;
	response->content_type = content_type;
	response->data = *data;
}

void http_respond(int clientfd, struct http_response* response) {
	char headers[1024];
	
	sprintf(headers, "HTTP/1.1\r\n");
	send(clientfd, headers, strlen(headers), 0);
	sprintf(headers, "Content-Type: %s\r\n", response->content_type);
	send(clientfd, headers, strlen(headers), 0);
	sprintf(headers, "Content-Length: %d\r\n", (int)response->content_length);
	send(clientfd, headers, strlen(headers), 0);
	sprintf(headers, "\r\n");
	send(clientfd, headers, strlen(headers), 0);
	send(clientfd, response->data, strlen(response->data), 0);
}