//
// Created by delinoz on 19-5-18.
//
#ifndef DIM_DIMCLIENT_H
#define DIM_DIMCLIENT_H

#include "DimProtocol.h"
#include "DateTimeUtils.h"
#include <cstring>
#include <vector>
#include <map>
#include <iostream>

#define EMPTY_RESPONSE_CONSUMER_ID 0

using std::cout;
using std::endl;
using std::strlen;
using std::strcpy;

class DimClient;

/**
 * 消耗响应
 */
class ResponseConsumer {
private:
    long id;
public:
    ResponseConsumer() {}

    ResponseConsumer(long id) {
        this->id = id;
    }

    long getId() {
        return this->id;
    }

    virtual void consume(DimPacket *packet, DimClient *client) = 0;
};

class EmptyResponseConsumer : public ResponseConsumer {
public:
    EmptyResponseConsumer() {}

    EmptyResponseConsumer(long id) : ResponseConsumer(id) {}

    virtual void consume(DimPacket *packet, DimClient *client) {}
};

class DimClient {
private:
    EmptyResponseConsumer *EMPTY_RESPONSE_CONSUMER;

    std::vector<MessageBox> boxes;
    std::map<long, ResponseConsumer &> responseConsumers;
    std::map<long, Peer> peers;
    Peer local;
    int port;
    pthread_t recvThread;
public:
    DimClient() {
        Peer peer;
        char local[16] = "127.0.0.1\0";
        //‘strlen’ was not declared in this scope
        // 添加cstring和定义std::
        strncpy(peer.ipv4, local, 16);
        peer.hashId = 141812760931708;
        peer.startTime = DateTimeUtils::getCurrentTime();
        this->port = peer.port = 10002;
        peers[141812760931708] = peer;
        EMPTY_RESPONSE_CONSUMER = new EmptyResponseConsumer(EMPTY_RESPONSE_CONSUMER_ID);
        memcpy(&this->local, &peer, sizeof(struct Peer));
    }

    //协议部分
    void start();

    int sendText(const DimTextMessage &);

    void receiver();

    int keepAlive(char *destIp);

    int connectByIpAndPort(char *destIp, int port);

    int responseConnectByIpAndPort(DimPacket &packet);

    int castIpUpdate();

    int responseCastIpUpdate();

    //处理packet
    DimPacket parsePacket(const DimTextMessage &message, char *buf);

    void resolvePacket(const DimPacket &packet, char *buf, int bufSize);

    void parsePacketHeader(DimPacket *packet, char *buf, int bufSize);

    void parsePacketData(char *packetData, const char *buf, int dataSize);

    int sendPacket(int sockfd, sockaddr_in *sockaddrIn, DimPacket &packet);

    //Peer的操作
    bool isContains(long hashId);

    bool addPeer(Peer peer);

    Peer remove(long hashId);

    Peer *get(long hashId);

    //事件处理
    ResponseConsumer &getConsumer(long id);

    void addConsumer(ResponseConsumer &consumer);

    void consume(DimPacket *packet, long id);

    //配置
    void setPort(int port);

    int getPort();

    void debug(DimPacket *packet);

    void debugIpFinderPacket(DimPacket *packet);
};


#endif //DIM_DIMCLIENT_H
