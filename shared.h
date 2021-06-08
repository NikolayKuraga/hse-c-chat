#ifndef SHARED_H
#define SHARED_H

// C standart libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// some POSIX libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define STR_LEN 1024
#define MAX_CMD_LEN 6
#define ARR_LEN(array, element) (sizeof(array) / sizeof(element)) // calculate length of array

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *src, void *dst);

#endif//SHARED_H
