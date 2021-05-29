#ifndef SERVER_H
#define SERVER_H

// standart thing
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// net, web and other communicative stuff
#include <windows.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

// multithreading from UNIX
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

// some macros
#define MAX_USER_INFO 20
#define MAX_USERS     10
#define STR_LEN       1024

// user-struct
typedef struct user {
    char login[MAX_USER_INFO];
    char password[MAX_USER_INFO];
} USER;

// array of users
typedef struct users {
    int n;
    USER usersInfo[MAX_USERS];
} USERS;

// some functions
void mutex_lock();
void mutex_unlock();
void* ClientStart(void* param);
int CreateServer();

#endif//SERVER_H
