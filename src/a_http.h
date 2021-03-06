#ifndef __A_HTTP_H__
#define __A_HTTP_H__

typedef struct http_request {
	char* method;
	char* path;
} http_request;

typedef struct http_response {
	char* data;
	int content_length;
	char* content_type;
} http_response;

void http_parse_request(struct http_request* request, char** buffer);
void http_set_response(struct http_response* response, char** data, char* content_type, int content_length);
void http_respond(int clientfd, struct http_response* response);
void http_read_request(int clientfd, char** buffer);
void http_resolve_content_type(char** result, char* filename);

#endif