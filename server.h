#ifndef SERVER_H
#define SERVER_H

#include "shared.h"
#include <pthread.h>

#define MAX_USER_INFO 20
#define MAX_USERS     10
#define STR_LEN       1024

typedef struct {
    char login[MAX_USER_INFO];
    char password[MAX_USER_INFO];
} User;

typedef struct {
    int n;
    User usersInfo[MAX_USERS];
} Users;

void server_mutex_lock();
void server_mutex_unlock();
void *ClientStart(void *fd);
int CreateServer();

#endif//SERVER_H
