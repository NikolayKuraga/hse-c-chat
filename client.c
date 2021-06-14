#include "client.h"

// Check for exit command, then terminate or do send() and return its out
int Send(SOCKET sockfd, const char *buf, size_t len, int flags) {
    if (!strcmp(buf, "exit")) {
        send(sockfd, "exit", sizeof("exit"), 0);
        printf("\r > exit\n");
        closesocket(sockfd);
        exit(EXIT_SUCCESS);
    }
    return send(sockfd, buf, len, flags);
}

void *ChatReceiverFun(void *fd) {
    ThreadKit *ChatReceiverKit = (ThreadKit *) fd;
    char receiveAr[STR_LEN] = { 0 };

    recv(ChatReceiverKit->sock, receiveAr, sizeof(receiveAr), 0); // without this line...
    for (int i; i > 0; i = recv(ChatReceiverKit->sock, receiveAr, sizeof(receiveAr), 0)) {
        printf("\r %s            \n", receiveAr); // ...this loop will run once at start...
        printf("\r %s > draft: ", ChatReceiverKit->username);// ... and it is not correct
    }
    return (void *) 0;
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

    char clientWelcomeMessage[STR_LEN];
    if (!strcmp(receiveAr, "success 1")) {
        strcpy(clientWelcomeMessage, "\rWelcome back, ");
        strcat(clientWelcomeMessage, username);
        strcat(clientWelcomeMessage, "!\n");
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
        strcpy(clientWelcomeMessage, "\rWelcome aboard, ");
        strcat(clientWelcomeMessage, username);
        strcat(clientWelcomeMessage, "!\n");
    }
    else if (!strcmp(receiveAr, "already logged in")) {
        printf("\rUser %s already logged in using another client launcher\n", username);
        closesocket(clientSock);
        return;
    }
    else {
        printf("\r\terror #1002");
        closesocket(clientSock);
        return;
    }

    inf = recv(clientSock, receiveAr, sizeof(receiveAr), 0);
    if (*receiveAr == 18) {
        printf("%s", clientWelcomeMessage);
        printf("\rChat history is empty. Wanna be first?\n");
    }
    else {
        printf("\r\n\tMessage history:\n\n");
        do {
            printf("\r %s", receiveAr);
            inf = recv(clientSock, receiveAr, sizeof(receiveAr), 0);
        } while (*receiveAr != 17);
        printf("\r%s", clientWelcomeMessage);
    }

//    pthread_mutex_init(&mutex, NULL);

    ThreadKit chatReceiverKit = { 0 };
    chatReceiverKit.sock = clientSock;
    strcpy(chatReceiverKit.username, username);
    pthread_t chatReceiverThread;
    pthread_create(&chatReceiverThread, NULL, ChatReceiverFun, (void *) &chatReceiverKit);
    pthread_detach(chatReceiverThread);

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
    }
}
