#ifndef SERVER_H
#define SERVER_H

#include "shared.h"
#include <stdarg.h>

// POSIX Threads (multithreading)
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#define REGISTERED_USERS_PATH    "./data/registered_users.txt"
#define SERVER_LOGS              "./data/logs.txt"
#define MESSAGE_HISTORY_PATH     "./data/message_history.txt"

#define MAX_USERS                5       // first user (user[0]) is empty (some kind of pattern)
#define STR_LEN                  1024

// Some list of users, for example list of registered users
typedef struct {
    int num;
    struct {
        int id;
        char username[STR_LEN];
        char password[STR_LEN];
    } user[MAX_USERS];
} UserList;

// Client socket starter kit -- a pack of important things. Its pointer is gived to newThread
typedef struct {
    int sock;
    int id;
    char username[STR_LEN];
    UserList *registered;
} ClientKit;

int CreateServer();

#endif//SERVER_H
