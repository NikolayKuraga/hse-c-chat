#include "server.h"

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

void *termFun() {
    char buf[STR_LEN] = { 0 };
    for(;;) {
        scanf("%s", buf);
        if (!strcmp(buf, "stop")) {
            exit(EXIT_SUCCESS);
        }
    }
}

void *ClientFun(void *fd) {
    int clientSock = (int) fd;
    char receiveAr[STR_LEN] = { 0 };
    char transmitAr[STR_LEN] = { 0 };
    char lastReceiveAr[STR_LEN] = { 0 };
    char userName[STR_LEN] = { 0 };
    char tmpAr[STR_LEN] = { 0 };
    int inf, interval;
/*
    inf =  read(clientSock, userName, sizeof(userName));
    if (inf == -1) {
        server_mutex_lock();
        printf("login failed\n");
        server_mutex_unlock();
        return (void *) 3;
    }
    printf("Hello!\n");
//    server_mutex_lock();
//    printf("user \"%s\" has logined", userName);
//    server_mutex_unlock();
*/
    inf =  read(clientSock, userName,  sizeof(userName));
    if (inf == -1) {
        server_mutex_lock();
        printf("login error #1\n");
        server_mutex_unlock();
        return (void *) 1;
    }

    server_mutex_lock();
    printf("new login: %s\n", userName);
    server_mutex_unlock();

    inf = write(clientSock, userName, sizeof(userName));
    if (inf == -1) {
        server_mutex_lock();
        printf("login error #2\n");
        server_mutex_unlock();
        return (void *) 2;
    }
/*
    for(;;) {
        inf =  read(clientSock, receiveAr,  sizeof(receiveAr));
        if (inf == -1) {
            server_mutex_lock();
            printf("error #1\n");
            server_mutex_unlock();
            return (void *) 1;
        }

        printf("last message: %s\n", lastReceiveAr);
        strcpy(lastReceiveAr, receiveAr);
        server_mutex_lock();
        printf("new message: %s\n", receiveAr);
        server_mutex_unlock();
        
        strcpy(transmitAr, "Your message \"");
        strcat(transmitAr, receiveAr);
        strcat(transmitAr, "\" has received");

        inf = write(clientSock, transmitAr, sizeof(transmitAr));
        if (inf == -1) {
            server_mutex_lock();
            printf("error #2");
            server_mutex_unlock();
            return (void *) 2;
        }
    }
*/
    return (void *) 0;
}

int CreateServer() {
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

    pthread_t termThread;
    pthread_create(&termThread, NULL, termFun, NULL);

    socklen_t serverAddrLen = sizeof(serverAddr);
    while (1) {
        clientSock = accept(serverSock, (struct sockaddr *) &clientAddr, &serverAddrLen);
        if (clientSock == -1) {
            server_mutex_lock();
            printf("accept failed\n");
            server_mutex_unlock();
            continue;
        }
        server_mutex_lock();
        printf("another client is accepted\n");
        server_mutex_unlock();
        pthread_t newThread;
        pthread_create(&newThread, NULL, ClientFun, (void *) clientSock);
        pthread_detach(newThread);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_file);
    printf("stop server\n");
    sleep(1);
    close(serverSock);
    return 0;
}
