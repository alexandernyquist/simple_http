#include <stdlib.h>
#include <stdio.h>
#include "a_utils.h"

void read_file(char* filename, char** buffer) {
	FILE* filefd;

	filefd = fopen(filename, "r");
	if(filefd == NULL)
		a_die("Could not open file");

	size_t nread;
	while((nread = fread(*buffer, 1, 1024, filefd)) > 0) {
	}

	fclose(filefd);
}

bool file_exists(char* filename) {
	FILE *file = fopen(filename, "r");
	if(file == NULL)
		return false;
	fclose(file);
	return true;
}