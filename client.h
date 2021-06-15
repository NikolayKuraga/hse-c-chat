#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

typedef struct {
    int sock;
    char username[STR_LEN];
} ThreadKit;

void CreateClient();

#endif//CLIENT_H
