#ifndef SHARED_H
#define SHARED_H

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// C standart libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// net, web and other communicative stuff
#include <ws2tcpip.h> // for structure socklen_t
#include <windows.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

// POSIX Threads (for multithreading)
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#define STR_LEN 1024
#define MAX_CMD_LEN 6
#define ARR_LEN(array, element) (sizeof(array) / sizeof(element)) // calculate length of array

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, int addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, int *addrlen);

void Connect(int sockfd, const struct sockaddr *addr, int addrlen);

#endif//SHARED_H
