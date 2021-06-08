#include "client.h"

// Check for exit command
ssize_t Write(int fd, const void *buf, size_t count) {
    if (strlen(buf) <= MAX_CMD_LEN && (!strcmp(buf, "exit") || !strcmp(buf, "exit\n"))) {
        write(fd, "exit", sizeof("exit"));
        printf("\r > exit\n");
        close(fd);
        exit(EXIT_SUCCESS);
    }
    return write(fd, buf, count);
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

    Connect(clientSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    printf("\rTo close client enter single word \"exit\"\n");

    char *p_tmpAr[2] = { username, password };
    char tmpAr[2][9] = { "username\0", "password\0" };
    for(int i = 0; i < 2; ++i) {
        printf("\rEnter %s: ", tmpAr[i]);
        fgets(p_tmpAr[i], sizeof(char) * STR_LEN, stdin);
        p_tmpAr[i][strlen(p_tmpAr[i]) - 1] = '\0';
        inf = Write(clientSock, p_tmpAr[i], sizeof(char) * STR_LEN);
        if (inf == -1) {
            printf("\rlogin error #%d (%s-step)\n", i + 1, tmpAr[i]);
            close(clientSock);
            return;
        }
    }

    inf = read(clientSock, receiveAr,  sizeof(receiveAr));
    if (inf == -1) {
        printf("\rlogin error #3 (reply-step)\n");
        close(clientSock);
        return;
    }

    if      (!strcmp(receiveAr, "success 1")) {
        printf("\rWelcome back, %s!\n", username);
    }
    else if (!strcmp(receiveAr, "wrong password")) {
        printf("\rWrong password for username %s.\n", username);
        close(clientSock);
        return;
    }
    else if (!strcmp(receiveAr, "not available")) {
        printf("\rRegistration is closed\n\n");
        close(clientSock);
        return;
    }
    else if (!strcmp(receiveAr, "success 2")) {
        printf("\rWelcome aboard, %s!\n", username);
    }
    else {
        printf("\rerror #1002");
        close(clientSock);
        return;
    }

    for(;;) {
        printf("\r %s > draft: ", username);
        fgets(transmitAr, ARR_LEN(transmitAr, *transmitAr), stdin);
        transmitAr[strlen(transmitAr) - 1] = '\0';
        inf = Write(clientSock, transmitAr, sizeof(receiveAr));
        if (inf == -1) {
            printf("\rerror #1\n");
            close(clientSock);
            return;
        }
        inf = read(clientSock, receiveAr,  sizeof(receiveAr));
        if (inf == -1) {
            printf("\rerror #2\n");
            close(clientSock);
            return;
        }
    }

    close(clientSock);
    return;
}
