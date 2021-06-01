#include "client.h"

void SendData2Server() {

    int number = 1;
    int count = 2;
    
    int clientSockFD = Socket(AF_INET, SOCK_STREAM, IPPROTO_IP/*0*/);
    struct sockaddr_in serverAddr = { 0 };
    int inf;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5510);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    Connect(clientSockFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    char message[1024];
    sprintf(message, "<%d client> %s %d", number, "test", count);
    inf = write(clientSockFD, message, sizeof(message) / sizeof(*message));
    if (inf == -1) {
        printf("Can't send message\n");
        close(clientSockFD);
        return;
    }
    printf("Sent: %s\n bytes: %d\n\n", message, inf);
    inf = -1;
    int i = 0;
    while (inf == -1) {
        inf =  read(clientSockFD, message, 1024);
        if (!inf || inf == -1) {
            printf("Connection closed\n");
            break;
        }
    }
/*    close(clientSockFD);
    write(fd, "Hello\n", 6);
    char buf[256];
    ssize_t nread;
    nread = read(fd, buf, 256);
    if (nread == -1) {
        perror("Read failed!");
        exit(EXIT_FAILURE);
    }
    if (nread == 0) {
        printf("EOF occured\n");
    }
    write(STDOUT_FILENO, buf, nread);
    close(fd);*/
}
