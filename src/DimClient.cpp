//
// Created by delinoz on 19-5-19.
//
#include <cstring>
#include "DimClient.h"
#include "UDPUtils.h"
#include "NetUtils.h"
#include "Arrays.h"
#include "DateTimeUtils.h"
#include "NumberUtils.h"

//协议
void DimClient::start() {
    receiver();
}

int DimClient::connectByIpAndPort(char *destIp, int port) {
    int sockfd = createSocket();
    DimPacket packet;
    packet.type = PACKET_TYPE_IP_FINDER;
    packet.srcHashId = local.hashId;
    packet.destHashId = 0;
    packet.sendTimeStamp = DateTimeUtils::getCurrentTime();
    packet.payloadLength = sizeof(DimIpFinderContent);

    DimIpFinderContent ipFinderContent;
    strncpy(ipFinderContent.destIpv4, destIp, strlen(destIp));
    strncpy(ipFinderContent.srcIpv4, local.ipv4, IPV4_ADDR_LENGTH);
    cout << "local.ipv4 " << local.ipv4 << endl;
    ipFinderContent.srcPort = local.port;
    ipFinderContent.srcIpv4[strlen(destIp)] = '\0';
    ipFinderContent.destHashId = 0;
    ipFinderContent.srcHashId = local.hashId;
    packet.content = (char *) &ipFinderContent;
    sockaddr_in sockaddrIn = NetUtils::getSockaddr_in(destIp, port);
    int packetSize = DIM_PACKET_HEADER_LENGTH + packet.payloadLength;
    debugIpFinderPacket(&packet);
    return sendPacket(sockfd, &sockaddrIn, packet);
}

void DimClient::receiver() {
    int sockfd = openServerSocket(port);
    int MAX_BUF = 1024;
    char buf[MAX_BUF];
    while (1) {
        cout << "recv data...." << endl;
        int recvSize = recv(sockfd, buf, MAX_BUF);
        DimPacket packet;
        char data[MAX_BUF];
        cout << "recv data size:" << recvSize << endl;
        if (recvSize > 0) {
            parsePacketHeader(&packet, buf, MAX_BUF);
            parsePacketData(data, buf, packet.payloadLength);
            data[packet.payloadLength] = '\0';
            packet.content = data;
            debug(&packet);
            consume(&packet, packet.type);
            debug(&packet);
        }
        memset(buf, 0, MAX_BUF);
    }
}

int DimClient::responseConnectByIpAndPort(DimPacket &packet) {
    int sockfd = createSocket();
    DimIpFinderContent *ipFinderContent = (DimIpFinderContent *) packet.content;
    NicInfoSet set;
    NetUtils::getLocalHostMacAddress(set);
    ipFinderContent->destHashId = NumberUtils::hexToLong(set.infos[0].mac, 12);
    long destHashId = packet.srcHashId;
    packet.destHashId = destHashId;
    packet.srcHashId = local.hashId;
    packet.type = PACKET_TYPE_IP_FINDER_RES;
    strncpy(ipFinderContent->destIpv4, local.ipv4, IPV4_ADDR_LENGTH);
    ipFinderContent->destPort = local.port;
    sockaddr_in sockaddrIn = NetUtils::getSockaddr_in(ipFinderContent->srcIpv4, ipFinderContent->srcPort);
    return sendPacket(sockfd, &sockaddrIn, packet);
}

int DimClient::castIpUpdate() {

}

int DimClient::responseCastIpUpdate() {

}

//包处理
DimPacket DimClient::parsePacket(const DimTextMessage &message, char *buf) {
    DimPacket packet;
    packet.type = message.type;
    packet.destHashId = message.destHashId;
    packet.srcHashId = local.hashId;
    packet.payloadLength = (message.text == nullptr ? 0 : strlen(message.text));
    packet.sendTimeStamp = DateTimeUtils::getCurrentTime();
    strncpy(buf, message.text, packet.payloadLength);
    packet.content = buf;
    return packet;
}

int DimClient::sendText(const DimTextMessage &message) {
    long destHashId = message.destHashId;
    Peer *peer = get(destHashId);
    if (peer == nullptr) {
        return -1;
    }
    int sockfd = createSocket();
    char buffer[128];
    DimPacket packet = parsePacket(message, buffer);
    sockaddr_in sockaddrIn = NetUtils::getSockaddr_in(peer->ipv4, peer->port);
    return sendPacket(sockfd, &sockaddrIn, packet);
}

int DimClient::sendPacket(int sockfd, sockaddr_in *sockaddrIn, DimPacket &packet) {
    int packetSize = DIM_PACKET_HEADER_LENGTH + packet.payloadLength;
    char buf[packetSize];
    debug(&packet);
    resolvePacket(packet, buf, packetSize);
    return send(sockfd, sockaddrIn, buf, packetSize);
}

void DimClient::resolvePacket(const DimPacket &packet, char *buf, int bufSize) {
    memcpy(buf, &packet, DIM_PACKET_HEADER_LENGTH);
    memcpy(buf + DIM_PACKET_HEADER_LENGTH, packet.content, packet.payloadLength);
}

void DimClient::parsePacketData(char *packetData, const char *buf, int dataSize) {
    memcpy(packetData, buf + DIM_PACKET_HEADER_LENGTH, dataSize);
}

void DimClient::parsePacketHeader(DimPacket *packet, char *buf, int bufSize) {
    memcpy(packet, buf, DIM_PACKET_HEADER_LENGTH);
}

int keepAlive(char *destIp);

DimPacket parseMessage(DimTextMessage);

//Peer的操作
bool isContains(long hashId);

bool addPeer(Peer peer);

Peer remove(long hashId);

Peer *DimClient::get(long hashId) {
    std::map<long, Peer>::iterator iter;

    iter = peers.find(hashId);
    if (iter == peers.end()) {
        return nullptr;
    }
    return &iter->second;
}

bool DimClient::addPeer(Peer peer) {
    peers.insert(std::make_pair(peer.hashId, peer));
}


void DimClient::debug(DimPacket *packet) {
    cout << "////////////////" << endl;
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<resolvePacket{ }" << endl;
    cout << "type:" << packet->type << endl;
    cout << "srcHashId:" << packet->srcHashId << endl;
    cout << "destHashId:" << packet->destHashId << endl;
    cout << "payloadLength:" << packet->payloadLength << endl;
    cout << "sendTimeStamp:" << packet->sendTimeStamp << endl;
    cout << "content:" << packet->content << endl;
    cout << "////////////////" << endl;
}

void DimClient::debugIpFinderPacket(DimPacket *packet) {
    cout << "parse Packet: " << endl;
    cout << "type:" << packet->type << endl;
    cout << "srcHashId:" << packet->srcHashId << endl;
    cout << "destHashId:" << packet->destHashId << endl;
    cout << "payloadLength:" << packet->payloadLength << endl;
    cout << "sendTimeStamp:" << packet->sendTimeStamp << endl;
    cout << "ipFinder content:" << endl;
    DimIpFinderContent *ipFinderContent = (DimIpFinderContent *) packet->content;
    cout << "destHashId:" << ipFinderContent->destHashId << endl;
    cout << "srcHashId:" << ipFinderContent->srcHashId << endl;
    cout << "srcIp:" << ipFinderContent->srcIpv4 << endl;
    cout << "destIp:" << ipFinderContent->destIpv4 << endl;
    cout << "local port:" << ipFinderContent->srcPort << endl;
}


//////////////////////////////////Response Process

//事件处理
void DimClient::addConsumer(ResponseConsumer &consumer) {
    responseConsumers.insert(std::pair<long, ResponseConsumer &>(consumer.getId(), consumer));
    cout << "map size : " << responseConsumers.size() << endl;
    cout << "consumer id : " << consumer.getId() << endl;
}

ResponseConsumer &DimClient::getConsumer(long id) {
    std::map<long, ResponseConsumer &>::iterator iter;

    iter = responseConsumers.find(id);
    if (iter == responseConsumers.end()) {
        return *EMPTY_RESPONSE_CONSUMER;
    }
    return iter->second;
}

void DimClient::consume(DimPacket *packet, long id) {
    ResponseConsumer &consumer = getConsumer(id);
    cout << consumer.getId() << endl;
    consumer.consume(packet, this);
}