#include "shared.h"

int Socket(int domain, int type, int protocol) {
    int inf = socket(domain, type, protocol);
    if (inf == -1) {
        perror("Socket failed!");
        exit(EXIT_FAILURE);
    }
    return inf;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int inf = bind(sockfd, addr, addrlen);
    if (inf == -1) {
        perror("Bind failed!");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog) {
    int inf = listen(sockfd, backlog);
    if (inf == -1) {
        perror("Listen failed!");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int inf = accept(sockfd, addr, addrlen);
    if (inf == -1) {
        perror("Accept failed!");
        exit(EXIT_FAILURE);
    }
    return inf;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int inf = connect(sockfd, addr, addrlen);
    if (inf == -1) {
        perror("Connect failed!");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char *src, void *dst) {
    int inf = inet_pton(af, src, dst);
    if (inf == 0) {
        printf("inet_pton failed: src does not contain a"
            " character string representing a valid"
            " network address in the specified address"
           " family\n");
        exit(EXIT_FAILURE);
    }
    if (inf == -1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
}
