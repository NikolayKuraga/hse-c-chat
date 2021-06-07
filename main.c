#include "server.h"
#include "client.h"

int main(int argc, char *argv[])
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
    {
        printf("Connected to socket lib\n");
    }
    else
    {
        printf("Can't connect to socket lib\n");
        return 1;
    }
    if (argc > 2) {
        printf("too many arguments\n\n");
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "server")) {
            printf("\n\tStart server\n\n");
            CreateServer();
        }
        else if (!strcmp(argv[1], "client")) {
            printf("\n\tStart client\n\n");
            CreateClient();
        }
        else {
            printf("wrong argument\n\n");
        }
    }
    else {
        printf("\n\tStart client by default\n\n");
        CreateClient();
    }
    return 0;
}
