#include "server.h"
#include "client.h"

int main(int argc, char *argv[])
{
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
