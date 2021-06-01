#include "server.h"

pthread_mutex_t mutex;
pthread_mutex_t mutex_file;

void server_mutex_lock() {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex_file);
}

void server_mutex_unlock() {
    pthread_mutex_unlock(&mutex_file);
    pthread_mutex_unlock(&mutex);
}

void *ClientStart(void *fd) {
    int clientSockFD = (int) fd;
    char receiveAr[1024], transmitAr[1024];
    int inf;

    strcpy(transmitAr, "Login: ");
    inf = write(clientSockFD, transmitAr, sizeof(transmitAr));
    if (inf == -1) {
        server_mutex_lock();
        printf("error #2");
        server_mutex_unlock();
        return (void *) 2;
    }

    inf =  read(clientSockFD, receiveAr,  sizeof(receiveAr));
    if (inf == -1) {
        server_mutex_lock();
        printf("error #1");
        server_mutex_unlock();
        return (void *) 1;
    }

    receiveAr[inf] = '\0';
    server_mutex_lock();
    printf("%s\n", receiveAr);
    server_mutex_unlock();
    sprintf(transmitAr, "%s %s %s\n", "Your data", receiveAr, "received");

    inf = write(clientSockFD, transmitAr, sizeof(transmitAr));
    if (inf == -1) {
        server_mutex_lock();
        printf("error #2");
        server_mutex_unlock();
        return (void *) 2;
    }

    return (void *) 0;
}

int CreateServer() {
    int serverSockFD = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP /*0*/);
    int clientSockFD = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP /*0*/);
    struct sockaddr_in serverAddr = { 0 };
    struct sockaddr_in clientAddr = { 0 };
    socklen_t serverAddrLen = sizeof(serverAddr);
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5510);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    



    Bind(serverSockFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr));







    Listen(serverSockFD, 5);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_file, NULL);

    while (1) {
        clientSockFD = Accept(serverSockFD, (struct sockaddr *) &clientAddr, &serverAddrLen);
        printf("Another client is accepted\n");
        pthread_t mythread;
        pthread_create(&mythread, NULL, ClientStart, (void *) clientSockFD);
        pthread_detach(mythread);
    }
    
    pthread_mutex_destroy(&mutex_file);
    pthread_mutex_destroy(&mutex);
    printf("Server is stopped\n");
    sleep(1);
    close(serverSockFD);
    return 0;
}
