//
// Created by delinoz on 19-5-18.
//

#ifndef DIM_UDPUTILS_H
#define DIM_UDPUTILS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <stdio.h>
#include <cstdlib>

int createSocket();

int bind(int sockfd, sockaddr_in *sockaddrIn);

int openServerSocket(int port);

struct sockaddr_in getSockaddr_in(const char *ip, int destPort);

int send(int sockfd, struct sockaddr_in *destAddr, void *data, int dataLength);

int recv(int sockfd, void *buf, int bufLength);

struct sockaddr_in getLocalSockaddr_in();

int recvfrom(int sockfd, void *buf, int bufLength, struct sockaddr_in *sockaddrIn);

#endif //DIM_UDPUTILS_H
