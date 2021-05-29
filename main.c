#include "server.h"
#include "client.h"

int main(int argc, char* argv[])
{
    if (argc > 0) {
        if (!strcmp(argv[1], "server")) {
            printf("SERVER\n");
        }
        else if (!strcmp(argv[1], "client")) {
            printf("CLIENT\n");
        }
        else {
            printf("WRONG ARGUMENT\n");
        }
    }
    else {
        printf("CLIENT\n");
    }
    return 0;
}
