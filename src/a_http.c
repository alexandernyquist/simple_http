#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "a_http.h"

void http_read_request(int clientfd, char** buffer) {
	recv(clientfd, *buffer, 1024, 0);
}

void http_parse_request(struct http_request* request, char** buffer) {
	request->method = malloc(sizeof(char*) * 255);
	request->path = malloc(sizeof(char*) * 255);

	char rest[255];
	sscanf(*buffer, "%s %s %s", request->method, request->path, rest);
}

void http_set_response(struct http_response* response, char** data, char* content_type, int content_length) {
	response->content_type = malloc(sizeof(char*) * 255);
	response->data = malloc(sizeof(char*) * 255);

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

void http_resolve_content_type(char** result, char* filename) {
	if(strstr(filename, ".html") != NULL) {
		sprintf(*result, "text/html");
	} else if(strstr(filename, ".css") != NULL) {
		sprintf(*result, "text/css");
	} else if(strstr(filename, ".js") != NULL) {
		sprintf(*result, "text/javascript");
	} else {
		sprintf(*result, "application/octet-stream");
	}
}