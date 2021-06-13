#include "shared.h"

int Socket(int domain, int type, int protocol) {
    int inf = socket(domain, type, protocol);
    if (inf == INVALID_SOCKET) {
        printf("socket failed\n");
        exit(EXIT_SUCCESS);
    }
    return inf;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int inf = bind(sockfd, addr, addrlen);
    if (inf == INVALID_SOCKET) {
        printf("bind failed\n");
        exit(EXIT_SUCCESS);
    }
}

void Listen(int sockfd, int backlog) {
    int inf = listen(sockfd, backlog);
    if (inf == INVALID_SOCKET) {
        printf("listen failed\n");
        exit(EXIT_SUCCESS);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int inf = accept(sockfd, addr, addrlen);
    if (inf == INVALID_SOCKET) {
        printf("accept failed\n");
        exit(EXIT_SUCCESS);
    }
    return inf;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int inf = connect(sockfd, addr, addrlen);
    if (inf == INVALID_SOCKET) {
        printf("connect failed\n");
        exit(EXIT_SUCCESS);
    }
}
