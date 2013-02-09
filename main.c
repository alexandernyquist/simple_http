#include <stdlib.h>
#include <stdio.h>
#include "a_utils.h"
#include "a_net.h"
#include "a_http.h"
#include "a_fs.h"

int main() 
{
	int port = 80;
	int sockfd;
	char* buffer = malloc(sizeof(char*) * 1024);
	char* filename = malloc(sizeof(char*) * 255);
	struct http_request* request = malloc(sizeof(*request));
	struct http_response* response = malloc(sizeof(*response));

	printf("Creating socket\n");
	sockfd = create_socket(port);

	printf("Listening for incoming connections\n");

	while(1) {
		int clientfd;
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
		sprintf(filename, "responses/%s", request->path);
		if(file_exists(filename) == false) {
			sprintf(filename, "responses/404.html");
		}

		memset(buffer, '\0', sizeof(char*) * 255);
		read_file(filename, &buffer);

		char* content_type = malloc(sizeof(char*) * 255);
		http_resolve_content_type(&content_type, filename);
		
		printf("[RESPONSE] Content-Type: %s\n", content_type);
		http_set_response(response, &buffer, content_type,  strlen(buffer));
		http_respond(clientfd, response);

		close(clientfd);

		printf("--------------\n");
	}

	free(response->data);
	free(response->content_type);
	free(response);

	free(request->method);
	free(request->path);
	free(request);
	free(filename);
	
	close(sockfd);

	return 0;
}
