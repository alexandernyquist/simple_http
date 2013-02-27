#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void a_die(char* msg) {
	printf("Error: %s\n", msg);
	printf("Errno: [%d]: %s\n", errno, (char*)strerror(errno));
	exit(0);
}