#include "shared.h"

int Socket(int domain, int type, int protocol) {
    int inf = socket(domain, type, protocol);
    if (inf == INVALID_SOCKET) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return inf;
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int inf = bind(sockfd, addr, addrlen);
    if (inf == INVALID_SOCKET) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog) {
    int inf = listen(sockfd, backlog);
    if (inf == INVALID_SOCKET) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    int inf = accept(sockfd, addr, addrlen);
    if (inf == INVALID_SOCKET) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return inf;
}

void Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int inf = connect(sockfd, addr, addrlen);
    if (inf == INVALID_SOCKET) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

void trim(char* str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char)str[begin])) {
        begin++;
    }

    while ((end >= begin) && isspace((unsigned char)str[end])) {
        end--;
    }

    for (i = begin; i <= end; i++) {
        str[i - begin] = str[i];
    }

    str[i - begin] = '\0';
}