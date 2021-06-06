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

// Terminal function. It expects some commands which server administrator can enter to server's terminal.
// Reacts to these commands. So, it is server's command line.
void * TermFun(void *fd) {
    ClientKit *p_userLists = (ClientKit *) fd;
    char cmd1[STR_LEN] = { 0 }; // first word in command, for example "print"
    char cmd2[STR_LEN] = { 0 }; // secomd word in command, for example "registered"
    char cmd3[STR_LEN] = { 0 }; // third word in command, for example "users"
    for(;;) {                                   // finally, "print registered users"
        scanf("%s", cmd1);
        if (!strcmp(cmd1, "stop")) {
            ServerPrint(3, "---\n > ", cmd1, "\n");
            FILE *wf = fopen(REGISTERED_USERS_PATH, "w+");
            for(int i = 1; p_userLists->registered->user[i].id && i < MAX_USERS; ++i) {
                fprintf(wf, "%4.1d : %s\n       %s\n\n", p_userLists->registered->user[i].id, p_userLists->registered->user[i].username, p_userLists->registered->user[i].password);
            }
            fclose(wf);
            server_mutex_lock();
            printf("---\n");
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(cmd1, "print")) {
            scanf("%s", cmd2);
            if (!strcmp(cmd2, "registered")) {
                scanf("%s", cmd3);
                if (!strcmp(cmd3, "users")) {
                    server_mutex_lock();
                    printf("---\n > %s %s %s\n", cmd1, cmd2, cmd3);
                    printf("number of users: %4.1d\n", p_userLists->registered->num);
                    for (int i = 1; p_userLists->registered->user[i].id && i < MAX_USERS; ++i) {
                        printf("user ID:       %4.1d\n", p_userLists->registered->user[i].id);
                        printf("     username: %s\n", p_userLists->registered->user[i].username);
                        printf("     password: %s\n", p_userLists->registered->user[i].password);
                    }
                    printf("---\n");
                    server_mutex_unlock();
                }
                else {
                    ServerPrint(7, "unknown command: \"", cmd1, " ", cmd2, " ", cmd3, "\" \n");
                }
            }
            else {
                ServerPrint(5, "unknown command: \"", cmd1, " ", cmd2, "\" \n");
            }
        }
        else {
            ServerPrint(3, "unknown command: \"", cmd1, "\" \n");
        }
    }
    
}

// Check for username and password. If here is not such username, then return 2. If here is such
// username, but password is not correct, then return 1. If here is such username and password is
// correct, then return 0.
int CheckForUser(char username[STR_LEN], char password[STR_LEN], UserList *userList) {
    for(int i = 1; i < MAX_USERS && userList->user[i].id; ++i) {
        if (!strcmp(username, userList->user[i].username)) {
            if (!strcmp(password, userList->user[i].password)) {
                return 0;
            }
            else {
                return 1;
            }
        }
    }
    return 2;
}

// Add user (username, password and id which is actually number of array element) to some user
// list. If user was added, then return 0. If user is not added (list is full), then return 1.
int AddUserToList(char username[STR_LEN], char password[STR_LEN], UserList *userList) {
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

// Thread function for client. It is called when new client's thread is creating.
void * ClientFun(void *fd) {
    ClientKit *p_clientKit = (ClientKit *) fd;
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char lastReceiveAr[STR_LEN] = { 0 };
    char username[STR_LEN] = { 0 };
    char password[STR_LEN] = { 0 };
    char tmpAr[STR_LEN] = { 0 };
    int inf;

    inf = read(p_clientKit->clientSock, username, sizeof(username));
    if (inf == -1) {
        ServerPrint(1, "error #1 (login username-step)\n");
        free(p_clientKit);
        return (void *) 1;
    }

    inf = read(p_clientKit->clientSock, password, sizeof(username));
    if (inf == -1) {
        ServerPrint(1, "error #2 (login password-step)\n");
        free(p_clientKit);
        return (void *) 2;
    }

    inf = CheckForUser(username, password, p_clientKit->registered);
    switch(inf) {
    case 0:
        ServerPrint(3, "user ", username, " has logged in\n");
        inf = write(p_clientKit->clientSock, "success 1", sizeof("success 1"));
        break;
    case 1:
        ServerPrint(3, "user ", username, " has tried to log in - wrong password\n");
        inf = write(p_clientKit->clientSock, "wrong password", sizeof("wrong password"));
        break;
    case 2:
        if (AddUserToList(username, password, p_clientKit->registered)) {
            ServerPrint(1, "error #4 (registration): list of registered users is full\n");
            inf = write(p_clientKit->clientSock, "not available", sizeof("not available"));
            free(p_clientKit);
            return (void *) 4;
        }
        ServerPrint(3, "user ", username, " has registered and logged in\n");
        inf = write(p_clientKit->clientSock, "success 2", sizeof("success 2"));
        break;
    default:
        ServerPrint(1, "error #1001");
        exit(0);
    }

    if (inf == -1) {
        ServerPrint(1, "error #3 (login reply-step)\n");
        free(p_clientKit);
        return (void *) 3;
    }


    for(;;) {
        inf = read(p_clientKit->clientSock, receiveAr,  sizeof(receiveAr));
        if (inf == -1) {
            server_mutex_lock();
            printf("error #1\n");
            server_mutex_unlock();
            return (void *) 1;
        }

        ServerPrint(3, "new message: ", receiveAr, "\n");
        
        strcpy(transmitAr, "Your message \"");
        strcat(transmitAr, receiveAr);
        strcat(transmitAr, "\" has received");

        inf = write(p_clientKit->clientSock, transmitAr, sizeof(transmitAr));
        if (inf == -1) {
            server_mutex_lock();
            printf("error #2");
            server_mutex_unlock();
            return (void *) 2;
        }
    }

    free(p_clientKit);
    return (void *) 0;
}

// Main server function
int CreateServer() {
    // "warning: missing braces around initializer" -- it is GCC bug 53119:
    UserList registered = { 0 };
    // to calm down GCC you should code like this: { 3, {{ 0 }} }

    int inf = 0;

    FILE *rf = fopen(REGISTERED_USERS_PATH, "r");
    if (rf) {
        int id = 0;
        char username[STR_LEN] = { 0 };
        char password[STR_LEN] = { 0 };
        for(int i = 1; i < MAX_USERS && fscanf(rf, "%d : %s\n %s", &id, username, password) != EOF; ++i) {
            ++registered.num;
            registered.user[i].id = id;
            strcpy(registered.user[i].username, username);
            strcpy(registered.user[i].password, password);
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

    ClientKit termFunCSSK = { 0, NULL, &registered };
    pthread_t termThread;
    pthread_create(&termThread, NULL, TermFun, (void *) &termFunCSSK);
    pthread_detach(termThread);

    socklen_t serverAddrLen = sizeof(serverAddr);
    for(;;) {
        clientSock = accept(serverSock, (struct sockaddr *) &clientAddr, &serverAddrLen);
        if (clientSock == -1) {
            ServerPrint(1, "accept failed\n");
            continue;
        }
        ServerPrint(1, "another client wants to login\n");
        
        ClientKit *p_clientKit = (ClientKit *) malloc(sizeof(ClientKit));
        p_clientKit->clientSock = clientSock;
        p_clientKit->active = NULL;
        p_clientKit->registered = &registered;
        
        pthread_t newThread;
        pthread_create(&newThread, NULL, ClientFun, (void *) p_clientKit);
        pthread_detach(newThread);
    }
/*
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_file);
    printf("stop server\n");
    sleep(1);
    close(serverSock);
*/
    return 0;
}
