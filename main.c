#include "server.h"
#include "client.h"

int main(int argc, char **argv)
{
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(1, 1), &wsd))
    {
        printf("socket lib error");
        exit(EXIT_SUCCESS);
    }
    if (argc > 2) {
        printf("too many arguments\n\n");
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "server")) {
            FILE* logs = fopen(SERVER_LOGS, "w");
            printf("\n\tStart server\n\n");
            fprintf(logs, "\n\tStart server\n\n");
            fclose(logs);
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
