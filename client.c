#include "client.h"

// Check for exit command
int Send(SOCKET sockfd, const char* buf, size_t len, int flags) {
    if (!strcmp(buf, "exit") || !strcmp(buf, "exit\n")) {
        send(sockfd, "exit", sizeof("exit"), 0);
        printf("\r > exit\n");
        closesocket(sockfd);
        exit(EXIT_SUCCESS);
    }
    return send(sockfd, buf, len, flags);
}
// Function for clienting or smth
void CreateClient() {
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char username[STR_LEN] = { 0 };
    char password[STR_LEN] = { 0 };
    int inf;

    int clientSock = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5510);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    Connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("\rTo close the client, enter \"exit\"\n");

    char* p_tmpAr[2] = { username, password };
    char tmpAr[2][10] = { "username\0", "password\0" };
    for (int i = 0; i < 2; ++i) {
        printf("\rEnter %s: ", tmpAr[i]);
        fgets(p_tmpAr[i], sizeof(char) * STR_LEN, stdin);
        p_tmpAr[i][strlen(p_tmpAr[i]) - 1] = '\0';
        trim(p_tmpAr[i]);
        inf = Send(clientSock, p_tmpAr[i], sizeof(char) * STR_LEN, 0);
        if (inf == SOCKET_ERROR) {
            printf("\r\tlogin error #%d (%s-step)\n", i + 1, tmpAr[i]);
            closesocket(clientSock);
            return;
        }
        if (!strlen(p_tmpAr[i])) {
            printf("\r\terror #%d (login %s-step): empty %s\n", i + 1, tmpAr[i], tmpAr[i]);
            closesocket(clientSock);
            return;
        }
    }

    inf = recv(clientSock, receiveAr, sizeof(receiveAr), 0);
    if (inf == SOCKET_ERROR) {
        printf("\r\tlogin error #3 (reply-step)\n");
        closesocket(clientSock);
        return;
    }

    if (!strcmp(receiveAr, "success 1")) {
        printf("\rWelcome back, %s!\n", username);
    }
    else if (!strcmp(receiveAr, "wrong password")) {
        printf("\rWrong password for username %s.\n", username);
        closesocket(clientSock);
        return;
    }
    else if (!strcmp(receiveAr, "not available")) {
        printf("\rRegistration is closed\n");
        closesocket(clientSock);
        return;
    }
    else if (!strcmp(receiveAr, "success 2")) {
        printf("\rWelcome aboard, %s!\n", username);
    }
    else {
        printf("\r\terror #1002");
        closesocket(clientSock);
        return;
    }

    for (;;) {
        printf("\r %s > draft: ", username);
        fgets(transmitAr, ARR_LEN(transmitAr, *transmitAr), stdin);
        transmitAr[strlen(transmitAr) - 1] = '\0';
        trim(transmitAr);
        inf = Send(clientSock, transmitAr, sizeof(transmitAr), 0);
        if (inf == SOCKET_ERROR) {
            printf("\r\terror #1\n");
            closesocket(clientSock);
            return;
        }
        inf = recv(clientSock, receiveAr, sizeof(receiveAr), 0);
        if (inf == SOCKET_ERROR) {
            printf("\r\terror #2\n");
            closesocket(clientSock);
            return;
        }
        if (!strcmp(receiveAr, "empty")) {
            printf("\r\tempty message! try again!\n");
        }
        else if (strcmp(receiveAr, "OK")) {
            printf("\r\terror #3: wrong reply from server\n");
        }
    }

    closesocket(clientSock);
    return;
}
