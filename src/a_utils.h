#ifndef __A_UTILS_H__
#define __A_UTILS_H__

#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0
#define bool int

/* general functionality */
void a_die(char* msg);

/* string functionality */
typedef struct {
	size_t len;
	char* raw;
} a_str_t;

#define a_str(str) { sizeof(str) - 1, (char*)str }

#endif