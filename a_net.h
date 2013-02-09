#ifndef __A_NET_H__
#define __A_NET_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "a_utils.h"

int create_socket(int port);
int accept_client(int sockfd);

#endif