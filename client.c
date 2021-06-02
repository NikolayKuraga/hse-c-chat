#include "client.h"

void CreateClient() {
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char userName[STR_LEN] = { 0 };
    int inf;

    int clientSock = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5510);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    Connect(clientSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

/*    printf("Enter some name: ");
    fgets(userName, ARR_LEN(userName, *userName), stdin);
    userName[strlen(userName) - 1] = '\0';
    inf = write(clientSock, userName, sizeof(userName));
    if (inf == -1) {
        printf("login failed\n");
        close(clientSock);
        return;
    }
*/

    printf("Enter some name: ");
    fgets(userName, ARR_LEN(userName, *userName), stdin);
    userName[strlen(userName) - 1] = '\0';
    inf = write(clientSock, userName, sizeof(userName));
    if (inf == -1) {
        printf("login error #1\n");
        close(clientSock);
        return;
    }
    inf =  read(clientSock, receiveAr,  sizeof(receiveAr));
    if (inf == -1) {
        printf("login error #2\n");
        close(clientSock);
        return;
    }
    printf("You logged in as %s\n", receiveAr);
    
/*
    for(;;) {
        printf(" %s > draft: ", userName);
        fgets(transmitAr, ARR_LEN(transmitAr, *transmitAr), stdin);
        transmitAr[strlen(transmitAr) - 1] = '\0';
        inf = write(clientSock, transmitAr, sizeof(receiveAr));
        if (inf == -1) {
            printf("error #1\n");
            close(clientSock);
            return;
        }
        inf =  read(clientSock, receiveAr,  sizeof(receiveAr));
        if (inf == -1) {
            printf("error #2\n");
            close(clientSock);
            return;
        }
        printf("message from server:\n\t<<<\n%s\n\t>>>\n", receiveAr);
    }
*/
    close(clientSock);
}
