#ifndef SERVER_H
#define SERVER_H

#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#define MAX_USER_INFO 20
#define MAX_USERS 10

typedef struct user
{
	char login[MAX_USER_INFO];
	char password[MAX_USER_INFO];
}USER;

typedef struct users
{
	int n;
	USER usersInfo[MAX_USERS];
}USERS;

void mutex_lock();
void mutex_unlock();
void* ClientStart(void* param);
int CreateServer();

#endif//!SERVER_H
