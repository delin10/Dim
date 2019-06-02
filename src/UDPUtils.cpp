//
// Created by delinoz on 19-5-18.
//
#include "UDPUtils.h"

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    return sockfd;
}

int bind(int sockfd, sockaddr_in *sockaddrIn) {
    int ret = 0;
    if ((ret = bind(sockfd, (struct sockaddr *) sockaddrIn, sizeof(struct sockaddr_in))) < 0) {
        perror("bind error!");
    }
    return ret;
}


int openServerSocket(int port) {
    struct sockaddr_in sockaddrIn;
    memset(&sockaddrIn, 0, sizeof(struct sockaddr_in));
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(port);
    sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    int sockfd = createSocket();
    if (bind(sockfd, &sockaddrIn) < 0) {
        //fail
        perror("error bind");
    }
    return sockfd;
}

int send(int sockfd, struct sockaddr_in *destAddr, void *data, int dataLength) {
    return sendto(sockfd, data, dataLength, 0,
                  (struct sockaddr *) destAddr, sizeof(struct sockaddr_in));
}

int recv(int sockfd, void *buf, int bufLength) {
    return recv(sockfd, buf, bufLength, 0);
}
