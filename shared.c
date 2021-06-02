#include "shared.h"

int Socket(int domain, int type, int protocol) {
    int inf = socket(domain, type, protocol);
    if (inf == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return inf;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int inf = bind(sockfd, addr, addrlen);
    if (inf == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog) {
    int inf = listen(sockfd, backlog);
    if (inf == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int inf = accept(sockfd, addr, addrlen);
    if (inf == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return inf;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int inf = connect(sockfd, addr, addrlen);
    if (inf == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char *src, void *dst) {
    int inf = inet_pton(af, src, dst);
    if (inf == -1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
}
