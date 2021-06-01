#include "server.h"
#include "client.h"

int main(int argc, char* argv[])
{
    if (argc > 1) {
        if (!strcmp(argv[1], "server")) {
            printf("SERVER\n");
            CreateServer();
        }
        else if (!strcmp(argv[1], "client")) {
            printf("CLIENT\n");
            SendData2Server();
        }
        else {
            printf("WRONG ARGUMENT\n");
        }
    }
    else {
        printf("CLIENT\n");
        SendData2Server();
    }
    return 0;
}

