#include "server.h"

// two mutexes and two functions for manipulating with server's multithreading
pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

void server_mutex_lock() {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex_file);
}

void server_mutex_unlock() {
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex_file);
}

// Print user list you give
void PrintUserList(UserList *p_userList) {
    server_mutex_lock();
    printf("\r\nnumber of users: %4.1d\n", p_userList->num);
    for (int i = 1; p_userList->user[i].id && i < MAX_USERS; ++i) {
        printf("user ID:       %4.1d\n", p_userList->user[i].id);
        printf("     username: %s\n", p_userList->user[i].username);
        printf("     password: %s\n", p_userList->user[i].password);
    } printf("\n");
    server_mutex_unlock();
}

// Lock all threads, print text to server's terminal and then unlock all threads
void ServerPrint(int args, ...) {
    va_list ap;
    va_start(ap, args);
    server_mutex_lock();
    for (int i = 0; i < args; ++i) {
        printf("%s", va_arg(ap, char *));
    }
    server_mutex_unlock();
    va_end(ap);
}

// Terminal function. It expects some commands which server administrator can enter
// to server's terminal. Reacts to these commands. So, it is server's command line.
void *TermFun(void *fd) {
    ClientKit *p_termKit = (ClientKit *) fd;
    char cmd1[STR_LEN] = { 0 };
    char cmd2[STR_LEN] = { 0 };
    char cmd3[STR_LEN] = { 0 };
    for (;;) {
        scanf("%s", cmd1);
        if (!strcmp(cmd1, "stop")) {
            server_mutex_lock();
            printf("\r---\n > %s\n---\n", cmd1);
            FILE *wf = fopen(REGISTERED_USERS_PATH, "w+");
            for (int i = 1; p_termKit->registered->user[i].id && i < MAX_USERS; ++i) {
                fprintf(wf, "%4.1d : %s\n       %s\n\n", p_termKit->registered->user[i].id, p_termKit->registered->user[i].username, p_termKit->registered->user[i].password);
            }
            fclose(wf);
            pthread_mutex_destroy(&mutex);
            pthread_mutex_destroy(&mutex_file);
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(cmd1, "print")) {
            scanf("%s", cmd2);
            if (!strcmp(cmd2, "registered") || !strcmp(cmd2, "active")) {
                scanf("%s", cmd3);
                if (!strcmp(cmd3, "users")) {
                    server_mutex_lock();
                    printf("\r---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    int j = 1;
                    if      (!strcmp(cmd2, "registered")) {
                        for (; p_termKit->registered->user[j].id && j < MAX_USERS; ++j) {
                            printf("user ID:       %4.1d\n", p_termKit->registered->user[j].id);
                            printf("     username: %s\n", p_termKit->registered->user[j].username);
                            printf("     password: %s\n", p_termKit->registered->user[j].password);
                        }
                    }
                    else if (!strcmp(cmd2, "active")) {
                        for (int i = 1; i < MAX_USERS; ++i) {
                            if (p_termKit->registered->user[i].stat == STAT_ONLINE) {
                                ++j;
                                printf("user ID:       %4.1d\n", p_termKit->registered->user[i].id);
                                printf("     username: %s\n", p_termKit->registered->user[i].username);
                                printf("     password: %s\n", p_termKit->registered->user[i].password);
                            }
                        }
                    }
                    printf("totally: %4.1d users\n", --j);
                    printf("---\n");
                    server_mutex_unlock();
                }
                else {
                    ServerPrint(7, "\runknown command: \"", cmd1, " ", cmd2, " ", cmd3, "\" \n");
                }
            }
            else {
                ServerPrint(5, "\runknown command: \"", cmd1, " ", cmd2, "\" \n");
            }
        }
        else if (!strcmp(cmd1, "clean")) {
            scanf("%s", cmd2);
            if (!strcmp(cmd2, "registered")) {
                scanf("%s", cmd3);
                if (!strcmp(cmd3, "users")) {
                    server_mutex_lock();
                    printf("\r---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    memset(p_termKit->registered, '\0', sizeof(UserList));
                    p_termKit->registered->num = 0;
                    fclose(fopen(REGISTERED_USERS_PATH, "w"));
                    printf("all registered users were deleted\n---\n"
                           "\n\tRestart server for correct working\n\n");
                    server_mutex_unlock();
                }
                else {
                    ServerPrint(7, "\runknown command: \"", cmd1, " ", cmd2, " ", cmd3, "\" \n");
                }
            }
            else if (!strcmp(cmd2, "messages")) {
                server_mutex_lock();
                printf("\r---\n > %s %s\n", cmd1, cmd2);
                fclose(fopen(CHAT_HISTORY_PATH, "w"));
                printf("message history was deleted\n---\n");
                server_mutex_unlock();
            }
            else {
                ServerPrint(5, "\runknown command: \"", cmd1, " ", cmd2, "\" \n");
            }
        }
        else {
            ServerPrint(3, "\runknown command: \"", cmd1, "\" \n");
        }
    }
}

// Check for username and password. If here is not such username, then return 0. If here
// is such username, but password is not correct, then return -1. If here is such username
// and password is correct, then return user's ID.
int CheckForUser(char username[STR_LEN], char password[STR_LEN], UserList* userList) {
    for (int i = 1; i < MAX_USERS; ++i) {
        if (!strcmp(username, userList->user[i].username)) {
            if (!strcmp(password, userList->user[i].password)) {
                return i; // return userList->user[i].id;
            }
            else {
                return -1;
            }
        }
    }
    return 0;
}

// Add user (username, password and id which is actually number of array element) to some user
// list. If user was added, then return its ID. If user is not added (list is full), then return 0.
int AddUserToList(char username[STR_LEN], char password[STR_LEN], UserList *userList) {
    if (userList->num < MAX_USERS - 1) {
        ++userList->num;
        userList->user[userList->num].id = userList->num;
        strcpy(userList->user[userList->num].username, username);
        strcpy(userList->user[userList->num].password, password);
        server_mutex_unlock();
        return userList->num;
    }
    return 0;
}

// Literaly remove client using its ClientKit and print message about it.
void RemClient(ClientKit *p_clientKit) {
    char buf[STR_LEN] = { 0 };
    sprintf(buf, "\rclient (socket=%d", p_clientKit->sock);
    if (*(p_clientKit->username) != '\0' && strcmp(p_clientKit->username, "exit")) {
        strcat(buf, ", username=");
        strcat(buf, p_clientKit->username);
    }
    server_mutex_lock();
    p_clientKit->registered->user[p_clientKit->id].stat = STAT_OFFLINE;
    closesocket(p_clientKit->sock);
    free(p_clientKit);
    printf("%s) has disconnected\n", buf);
    server_mutex_unlock();
    return;
}

// Thread function for client. It is called when new client's thread is creating.
void *ClientFun(void *fd) {
    ClientKit *p_clientKit = (ClientKit *) fd;
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char username[STR_LEN] = { 0 };
    char password[STR_LEN] = { 0 };
    char buf[STR_LEN * 10] = { 0 };  // some array as long as ten STR_LEN-long arrays
    int inf, out;

    char *p_tmpAr[2] = { username, password };
    char tmpAr[2][10] = { "username\0", "password\0" };
    for (int i = 0; i < 2; ++i) {
        inf = recv(p_clientKit->sock, p_tmpAr[i], sizeof(char) * STR_LEN, 0);
        if (inf == SOCKET_ERROR || (strlen(p_tmpAr[i]) <= MAX_CMD_LEN && !strcmp(p_tmpAr[i], "exit"))) {
            RemClient(p_clientKit);
            if (inf == SOCKET_ERROR) {
                ServerPrint(1, "\r\terror #%d (login%s-step)\n", i + 1, tmpAr[i]);
                return (void *) 1;
            }
            return (void *) 0;
        }
    }
    strcpy(p_clientKit->username, username);

    server_mutex_lock();
    p_clientKit->id = CheckForUser(p_clientKit->username, password, p_clientKit->registered);
    out = 0;
    if      (p_clientKit->id > 0) {
        printf("\ruser %s", p_clientKit->username);
        if (p_clientKit->registered->user[p_clientKit->id].stat == STAT_ONLINE) {
            printf(" tried to log in - already logged in\n");
            inf = send(p_clientKit->sock, "already logged in", sizeof("already logged in"), 0);
            out += 2;
        }
        else {
            printf(" logged in\n");
            p_clientKit->registered->user[p_clientKit->id].stat = STAT_ONLINE;
            inf = send(p_clientKit->sock, "success 1", sizeof("success 1"), 0);
        }
    }
    else if (p_clientKit->id == 0) {
        p_clientKit->id = AddUserToList(p_clientKit->username, password, p_clientKit->registered);
        if (p_clientKit->id == 0) {
            printf("\rlist of registered users is full, registration is closed\n");
            inf = send(p_clientKit->sock, "not available", sizeof("not available"), 0);
            ++out;
        }
        else {
            printf("\ruser %s has registered and logged in\n", p_clientKit->username);
            p_clientKit->registered->user[p_clientKit->id].stat = STAT_ONLINE;
            inf = send(p_clientKit->sock, "success 2", sizeof("success 2"), 0);
        }
    }
    else {
        printf("\ruser %s has tried to log in - wrong password\n", p_clientKit->username);
        inf = send(p_clientKit->sock, "wrong password", sizeof("wrong password"), 0);
        ++out;
    }
    p_clientKit->registered->user[p_clientKit->id].curSock = p_clientKit->sock;
    server_mutex_unlock();

    int *p_curClientStat = &(p_clientKit->registered->user[p_clientKit->id].stat);
    if (out || inf == SOCKET_ERROR) {
        RemClient(p_clientKit);
        if (out == 2) {
            *p_curClientStat = STAT_ONLINE;
        }
        if (inf == SOCKET_ERROR) {
            ServerPrint(1, "\r\terror #3 (login reply-step)\n");
            return (void *) 1;
        }
        return (void *) 0;
    }

    *transmitAr = 18;
    server_mutex_lock();
    FILE *chatHistory = fopen(CHAT_HISTORY_PATH, "r");
    if (chatHistory) {
        fseek(chatHistory, 0, SEEK_END);
        if (ftell(chatHistory) > 0) {
            fseek(chatHistory, 0, SEEK_SET);
            while (fgets(transmitAr, STR_LEN, chatHistory) != NULL) {
                inf = send(p_clientKit->sock, transmitAr, sizeof(transmitAr), 0);
            }
            *transmitAr = 17;
        }
        fclose(chatHistory);
    }
    send(p_clientKit->sock, transmitAr, sizeof(transmitAr), 0);

    for (;;) {
        server_mutex_unlock();
        inf = recv(p_clientKit->sock, receiveAr, sizeof(receiveAr), 0);
        if (inf == SOCKET_ERROR || !strcmp(receiveAr, "exit")) {
            RemClient(p_clientKit);
            if (inf == SOCKET_ERROR) {
                ServerPrint(1, "\r\terror #1\n");
                return (void *) 1;
            }
            return (void *) 0;
        }

        sprintf(buf, "%s: %s", p_clientKit->username, receiveAr);
        strncpy(transmitAr, buf, sizeof(transmitAr));
        transmitAr[ARR_LEN(transmitAr, *transmitAr) - 1] = '\0';
        server_mutex_lock();
        printf("\r %s\n", transmitAr);
        FILE *messageHistory = fopen(CHAT_HISTORY_PATH, "a");
        fprintf(messageHistory, "%s\n", transmitAr);
        fclose(messageHistory);
        for (int i = 1; i < MAX_USERS; ++i) {
            if (p_clientKit->registered->user[i].stat == STAT_ONLINE  &&
                p_clientKit->registered->user[i].id   != p_clientKit->id) {
                send(p_clientKit->registered->user[i].curSock, transmitAr, sizeof(transmitAr), 0);
            }
        }
    }
}

// Main server function
int CreateServer() {
    UserList registered = { 0 };

    FILE *rf = fopen(REGISTERED_USERS_PATH, "r");
    if (rf) {
        int id = 0;
        char username[STR_LEN] = { 0 };
        char password[STR_LEN] = { 0 };
        for (int i = 1; i < MAX_USERS && fscanf(rf, "%d : %s\n %s", &id, username, password) != EOF; ++i) {
            if (strcmp(registered.user[i - 1].username, username)) {
                ++registered.num;
                registered.user[i].id = id;
                strcpy(registered.user[i].username, username);
                strcpy(registered.user[i].password, password);
                registered.user[i].stat = STAT_OFFLINE;
            }
        }
        fclose(rf);
    }

    int serverSock = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    int clientSock = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in serverAddr = { 0 };
    struct sockaddr_in clientAddr = { 0 };

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5510);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    Bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    Listen(serverSock, 5);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);

    ClientKit termKit = { 0, 0, { 0 }, &registered };
    pthread_t termThread;
    pthread_create(&termThread, NULL, TermFun, (void *) &termKit);
    pthread_detach(termThread);

    socklen_t serverAddrLen = sizeof(serverAddr);
    for (;;) {
        clientSock = accept(serverSock, (struct sockaddr *) &clientAddr, &serverAddrLen);
        if (clientSock == INVALID_SOCKET) {
            ServerPrint(1, "\raccept failed\n");
            continue;
        }

        server_mutex_lock();
        printf("\rclient (socket=%d) has connected, wants to login\n", clientSock);
        server_mutex_unlock();

        ClientKit *p_clientKit = (ClientKit *) malloc(sizeof(ClientKit));
        p_clientKit->sock = clientSock;
        p_clientKit->id = 0;
        memset(p_clientKit->username, '\0', sizeof(p_clientKit->username));
        p_clientKit->registered = &registered;

        pthread_t newThread;
        pthread_create(&newThread, NULL, ClientFun, (void *) p_clientKit);
        pthread_detach(newThread);
    }
}
