#include "client.h"

void CreateClient() {
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char username[STR_LEN] = { 0 };
    char tmpAr[STR_LEN] = { 0 };
    int inf;

    int clientSock = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5510);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    Connect(clientSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    printf("Enter username: ");
    fgets(username, ARR_LEN(username, *username), stdin);
    username[strlen(username) - 1] = '\0';
    inf = send(clientSock, username, sizeof(username), 0);
    if (inf == SOCKET_ERROR) {
        printf("login error #1 (username-step)\n");
        closesocket(clientSock);
        return;
    }
    printf("Enter password: ");
    fgets(tmpAr, ARR_LEN(tmpAr, *tmpAr), stdin);
    tmpAr[strlen(tmpAr) - 1] = '\0';
    inf = send(clientSock, tmpAr, sizeof(tmpAr), 0);
    if (inf == SOCKET_ERROR) {
        printf("login error #2 (password-step)\n");
        closesocket(clientSock);
        return;
    }
    inf = recv(clientSock, receiveAr, sizeof(receiveAr), 0);
    if (inf == SOCKET_ERROR) {
        printf("login error #3 (reply-step)\n");
        closesocket(clientSock);
        return;
    }

    if (!strcmp(receiveAr, "success 1")) {
        printf("Welcome back, %s!\n", username);
    }
    else if (!strcmp(receiveAr, "wrong password")) {
        printf("Wrong password for username %s.\n", username);
        closesocket(clientSock);
        return;
    }
    else if (!strcmp(receiveAr, "not available")) {
        printf("Registration is closed\n");
        closesocket(clientSock);
        return;
    }
    else if (!strcmp(receiveAr, "success 2")) {
        printf("Welcome aboard, %s!\n", username);
    }
    else {
        printf("error #1002");
    }

    for(;;) {
        printf(" %s > draft: ", username);
        fgets(transmitAr, ARR_LEN(transmitAr, *transmitAr), stdin);
        transmitAr[strlen(transmitAr) - 1] = '\0';
        inf = send(clientSock, transmitAr, sizeof(receiveAr), 0);
        if (inf == SOCKET_ERROR) {
            printf("error #1\n");
            closesocket(clientSock);
            return;
        }
        inf =  recv(clientSock, receiveAr,  sizeof(receiveAr), 0);
        if (inf == SOCKET_ERROR) {
            printf("error #2\n");
            closesocket(clientSock);
            return;
        }
        printf("message from server:\n\t<<<\n%s\n\t>>>\n", receiveAr);
    }

    closesocket(clientSock);
}
