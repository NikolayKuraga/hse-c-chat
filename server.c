#include "server.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

// lock all threads until server_mutex_unlock is called
void server_mutex_lock() {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex_file);
}

// unlock all threads until server_mutex_lock is called
void server_mutex_unlock() {
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex_file);
}

// Print user list you give
void PrintUserList(UserList* p_userList) {
    server_mutex_lock();
    printf("\rnumber of users: %4.1d\n", p_userList->num);
    FILE* logs = fopen(SERVER_LOGS, "a");
    fprintf(logs, "\rnumber of users: %4.1d\n", p_userList->num);
    for (int i = 1; p_userList->user[i].id && i < MAX_USERS; ++i) {
        printf("user ID:       %4.1d\n", p_userList->user[i].id);
        printf("     username: %s\n", p_userList->user[i].username);
        printf("     password: %s\n", p_userList->user[i].password);
        fprintf(logs, "user ID:       %4.1d\n     username: %s\n     password: %s\n", p_userList->user[i].id, p_userList->user[i].username, p_userList->user[i].password);
    } printf("\n");
    fclose(logs);
    server_mutex_unlock();
}

// Lock all threads, print text to server's terminal and then unlock all threads
void ServerPrint(int args, ...) {
    va_list ap;
    va_list ap2;
    va_start(ap, args);
    va_start(ap2, args);
    server_mutex_lock();
    FILE* logs = fopen(SERVER_LOGS, "a");

    for (int i = 0; i < args; ++i) {
        printf("%s", va_arg(ap, char*));
        fprintf(logs, "%s", va_arg(ap2, char*));
    }
    fclose(logs);
    server_mutex_unlock();
    va_end(ap);
}

// Terminal function. It expects some commands which server administrator can enter
// to server's terminal. Reacts to these commands. So, it is server's command line.
void* TermFun(void* fd) {
    ClientKit* p_termKit = (ClientKit*)fd;
    char cmd1[STR_LEN] = { 0 };
    char cmd2[STR_LEN] = { 0 };
    char cmd3[STR_LEN] = { 0 };
    for (;;) {
        scanf("%s", cmd1);
        if (!strcmp(cmd1, "stop")) {
            server_mutex_lock();
            printf("\r---\n > %s\n---\n", cmd1);
            FILE* wf = fopen(REGISTERED_USERS_PATH, "w+");
            FILE* logs = fopen(SERVER_LOGS, "a");
            fprintf(logs, "\r---\n > %s\n---\n", cmd1);

            fclose(logs);
            for (int i = 1; p_termKit->registered->user[i].id && i <= MAX_USERS; ++i) {
                fprintf(wf, "%4.1d : %s\n       %s\n\n", p_termKit->registered->user[i].id, p_termKit->registered->user[i].username, p_termKit->registered->user[i].password);
            }

            fclose(wf);

            pthread_mutex_destroy(&mutex);
            pthread_mutex_destroy(&mutex_file);
            closesocket(p_termKit->sock);
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(cmd1, "print")) {
            scanf("%s", cmd2);
            if (!strcmp(cmd2, "registered")) {
                scanf("%s", cmd3);
                if (!strcmp(cmd3, "users")) {
                    server_mutex_lock();
                    FILE* logs = fopen(SERVER_LOGS, "a");
                    printf("\r---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    printf("number of users: %4.1d\n", p_termKit->registered->num);
                    fprintf(logs, "\r---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    fprintf(logs, "number of users: %4.1d\n", p_termKit->registered->num);


                    for (int i = 1; p_termKit->registered->user[i].id && i < MAX_USERS; ++i) {
                        printf("user ID:       %4.1d\n", p_termKit->registered->user[i].id);
                        printf("     username: %s\n", p_termKit->registered->user[i].username);
                        printf("     password: %s\n", p_termKit->registered->user[i].password);
                        fprintf(logs, "user ID:       %4.1d\n     username: %s\n     password: %s\n", p_termKit->registered->user[i].id, p_termKit->registered->user[i].username, p_termKit->registered->user[i].password);
                    }
                    printf("---\n");
                    fprintf(logs, "---\n");
                    fclose(logs);
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
                    FILE* logs = fopen(SERVER_LOGS, "a");
                    printf("\r---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    fprintf(logs, "\r---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    for (int i = 1; p_termKit->registered->user[i].id && i < MAX_USERS; ++i) {
                        p_termKit->registered->user[i].id = 0;
                        memset(p_termKit->registered->user[i].username, '\0',
                            sizeof(p_termKit->registered->user[i].username));
                        memset(p_termKit->registered->user[i].password, '\0',
                            sizeof(p_termKit->registered->user[i].password));
                    }
                    p_termKit->registered->num = 0;
                    fclose(fopen(REGISTERED_USERS_PATH, "w"));
                    ServerPrint(1, "all registered users were deleted\n---\n");



                    fclose(logs);

                }
                else {
                    ServerPrint(7, "unknown command: \"", cmd1, " ", cmd2, " ", cmd3, "\" \n");

                }
            }
            else if (!strcmp(cmd2, "messages")) {
                FILE* logs = fopen(SERVER_LOGS, "a");
                printf("\r---\n > %s %s\n", cmd1, cmd2);
                fprintf(logs, "\r---\n > %s %s\n", cmd1, cmd2);
                fclose(logs);
                fclose(fopen(MESSAGE_HISTORY_PATH, "w"));
                ServerPrint(1, "message history was deleted\n---\n");

            }
            else {
                ServerPrint(5, "unknown command: \"", cmd1, " ", cmd2, "\" \n");
            }
        }
        else {
            ServerPrint(3, "\runknown command: \"", cmd1, "\" \n");
        }
    }
}

// Check for username and password. If here is not such username, then return -2. If here
// is such username, but password is not correct, then return -1. If here is such username
// and password is correct, then return 0.
int CheckForUser(char username[STR_LEN], char password[STR_LEN], UserList* userList) {
    for (int i = 1; i < MAX_USERS && userList->user[i].id; ++i) {
        if (!strcmp(username, userList->user[i].username)) {
            if (!strcmp(password, userList->user[i].password)) {
                return 0;
            }
            else {
                return -1;
            }
        }
    }
    return -2;
}

// Add user (username, password and id which is actually number of array element) to some user
// list. If user was added, then return 0. If user is not added (list is full), then return 1.
int AddUserToList(char username[STR_LEN], char password[STR_LEN], UserList* userList) {
    server_mutex_lock();
    if (userList->num < MAX_USERS - 1) {
        ++userList->num;
        userList->user[userList->num].id = userList->num;
        strcpy(userList->user[userList->num].username, username);
        strcpy(userList->user[userList->num].password, password);
        server_mutex_unlock();
        return 0;
    }
    server_mutex_unlock();
    return 1;
}

// Literaly remove client using its ClientKit. Print message, close socket and free() ClientKit
void RemClient(ClientKit* p_clientKit) {
    char buf[STR_LEN * 2] = { 0 };
    sprintf(buf, "\rclient (socket=%d", p_clientKit->sock);
    if (*(p_clientKit->username) != '\0' && strcmp(p_clientKit->username, "exit")) {
        strcat(buf, ", username=");
        strcat(buf, p_clientKit->username);
    }
    ServerPrint(2, buf, ") has disconnected\n");
    closesocket(p_clientKit->sock);
    free(p_clientKit);
    return;
}

// Thread function for client. It is called when new client's thread is creating.
void* ClientFun(void* fd) {
    ClientKit* p_clientKit = (ClientKit*)fd;
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char username[STR_LEN] = { 0 };
    char password[STR_LEN] = { 0 };
    int inf, out;

    char* p_tmpAr[2] = { username, password };
    char tmpAr[2][10] = { "username\0", "password\0" };
    for (int i = 0; i < 2; ++i) {
        inf = recv(p_clientKit->sock, p_tmpAr[i], sizeof(char) * STR_LEN, 0);
        if (inf == SOCKET_ERROR || (strlen(p_tmpAr[i]) <= MAX_CMD_LEN && !strcmp(p_tmpAr[i], "exit"))) {
            RemClient(p_clientKit);
            if (inf == SOCKET_ERROR) {
                ServerPrint(1, "\r\terror #%d (login%s-step)\n", i + 1, tmpAr[i]);
                return (void*)1;
            }
            return (void*)0;
        }
    }
    strcpy(p_clientKit->username, username);

    inf = CheckForUser(p_clientKit->username, password, p_clientKit->registered);
    out = 1;
    switch (inf) {
    case 0:
        ServerPrint(3, "\ruser ", p_clientKit->username, " has logged in\n");
        inf = send(p_clientKit->sock, "success 1", sizeof("success 1"), 0);
        break;
    case -1:
        ServerPrint(3, "\ruser ", p_clientKit->username, " has tried to log in - wrong password");
        inf = send(p_clientKit->sock, "wrong password", sizeof("wrong password"), 0);
        out = 0;
        break;
    case -2:
        if (AddUserToList(p_clientKit->username, password, p_clientKit->registered)) {
            ServerPrint(1, "\rlist of registered users is full, registration is closed");
            inf = send(p_clientKit->sock, "not available", sizeof("not available"), 0);
            out = 0;
        }
        else {
            ServerPrint(3, "\ruser ", p_clientKit->username, " has registered and logged in");
            inf = send(p_clientKit->sock, "success 2", sizeof("success 2"), 0);
        }
        break;
    default:
        RemClient(p_clientKit);
        ServerPrint(1, "\r\terror #1001");
        exit(0);
    }

    if (!out || inf == SOCKET_ERROR) {
        RemClient(p_clientKit);
        if (inf == SOCKET_ERROR) {
            ServerPrint(1, "\r\terror #3 (login reply-step)\n");
            return (void*)1;
        }
        return (void*)0;
    }

    for (;;) {
        inf = recv(p_clientKit->sock, receiveAr, sizeof(receiveAr), 0);
        if (inf == SOCKET_ERROR || !strcmp(receiveAr, "exit")) {
            RemClient(p_clientKit);
            if (inf == SOCKET_ERROR) {
                ServerPrint(1, "\r\terror #1\n");
                return (void*)1;
            }
            return (void*)0;
        }

        server_mutex_lock();
        printf("\r %s: %s\n", p_clientKit->username, receiveAr);
        FILE* messageHistory = fopen(MESSAGE_HISTORY_PATH, "a");
        fprintf(messageHistory, "%s: %s\n", p_clientKit->username, receiveAr);
        fclose(messageHistory);
        FILE* logs = fopen(SERVER_LOGS, "a");
        fprintf(logs, "\n %s: %s", p_clientKit->username, receiveAr);
        fclose(logs);
        server_mutex_unlock();

        strcpy(transmitAr, "OK");
        inf = send(p_clientKit->sock, transmitAr, sizeof(transmitAr), 0);
        if (inf == SOCKET_ERROR) {
            ServerPrint(1, "\r\terror #2\n");
            return (void*)2;
        }
    }

    RemClient(p_clientKit);
    return (void*)0;
}

// Main server function
int CreateServer() {
    UserList registered = { 0 };

    FILE* rf = fopen(REGISTERED_USERS_PATH, "r");
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

    Bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    Listen(serverSock, 5);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);

    ClientKit termKit = { 0, 0, { 0 }, &registered };
    pthread_t termThread;
    pthread_create(&termThread, NULL, TermFun, (void*)&termKit);
    pthread_detach(termThread);

    socklen_t serverAddrLen = sizeof(serverAddr);
    for (;;) {
        clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &serverAddrLen);
        if (clientSock == INVALID_SOCKET) {
            ServerPrint(1, "\raccept failed\n");
            continue;
        }

        server_mutex_lock();
        printf("\rclient (socket=%d) has connected, wants to login\n", clientSock);

        FILE* logs = fopen(SERVER_LOGS, "a");
        fprintf(logs, "\rclient (socket=%d) has connected, wants to login", clientSock);
        fclose(logs);

        server_mutex_unlock();

        ClientKit* p_clientKit = (ClientKit*)malloc(sizeof(ClientKit));
        p_clientKit->sock = clientSock;
        p_clientKit->id = 0;
        memset(p_clientKit->username, '\0', sizeof(p_clientKit->username));
        p_clientKit->registered = &registered;

        pthread_t newThread;
        pthread_create(&newThread, NULL, ClientFun, (void*)p_clientKit);
        pthread_detach(newThread);

    }

}