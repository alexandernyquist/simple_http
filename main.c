#include <stdlib.h>
#include <stdio.h>
#include "a_utils.h"
#include "a_net.h"
#include "a_http.h"
#include "a_fs.h"

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

	while(1) {
		clientfd = accept_client(sockfd);
		printf("Client connected\n");

		http_read_request(clientfd, &buffer);
		http_parse_request(request, &buffer);

		// If / is requested, substitute it for index
		if(strlen(request->path) == 1) {
			sprintf(request->path, "index");
		}

		// Make sure path not containing . ends with .html
		if(strstr(request->path, ".") == NULL) {
			strcat(request->path, ".html");
		}

		printf("[REQUEST] Method: %s\n", request->method);
		printf("[REQUEST] Path: %s\n", request->path);

		// Make sure that requested file exists
		char* filename = (char*)malloc(1024);
		sprintf(filename, "responses/%s", request->path);
		if(file_exists(filename) == false) {
			sprintf(filename, "responses/404.html");
		}

		memset(buffer, 0, 1024);
		read_file(filename, &buffer);

		char* content_type = (char*)malloc(sizeof(char) * 255);
		if(strstr(filename, ".html") != NULL) {
			sprintf(content_type, "text/html");
		} else if(strstr(filename, ".css") != NULL) {
			sprintf(content_type, "text/css");
		} else if(strstr(filename, ".js") != NULL) {
			sprintf(content_type, "text/javascript");
		} else {
			sprintf(content_type, "application/octet-stream");
		}

		printf("[RESPONSE] Content-Type: %s\n", content_type);
		http_set_response(response, &buffer, content_type,  strlen(buffer));
		http_respond(clientfd, response);

		close(clientfd);
		free(filename);

		printf("--------------\n");
	}
	
	close(sockfd);	

	free(buffer);
	free(request);
	free(response);

	return 0;
}
