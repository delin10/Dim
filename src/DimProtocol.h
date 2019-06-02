//
// Created by delinoz on 19-5-18.
//

#ifndef DIM_DIMPROTOCOL_H
#define DIM_DIMPROTOCOL_H

#include "Constants.h"
#include <vector>
//DimPacket Type
#define PACKET_TYPE_KEEP_ALIVE      0b00000000
#define PACKET_TYPE_ONLINE          0b00000001
#define PACKET_TYPE_TEXT            0b00000010
#define PACKET_TYPE_FILE            0b00000011

#define PACKET_TYPE_IP_FINDER       0b01000000
#define PACKET_TYPE_IP_FINDER_RES   0b11000000
#define PACKET_TYPE_IP_REPORT       0b01000001

//DimProtocol协议包
typedef struct DimPacket {
    char type;//8
    long sendTimeStamp;
    long srcHashId;
    long destHashId;
    int payloadLength;
    char *content;
} DimPacket;

typedef struct DimIpFinderContent {
    char srcIpv4[IPV4_ADDR_LENGTH];
    char destIpv4[IPV4_ADDR_LENGTH];
    long srcHashId;
    long destHashId;
    int srcPort;
    int destPort;
} DimIpFinderContent;
//Dim 文本消息
typedef struct DimTextMessage {
    char type;
    long destHashId;
    char *text;
} DimTextMessage;

typedef struct Peer {
    long hashId;
    char ipv4[IPV4_ADDR_LENGTH];
    long startTime;
    int port;
} Peer;

typedef struct MessageBox {
    Peer peer;
    std::vector<DimTextMessage> box;
} MessageBox;

static int DIM_PACKET_HEADER_LENGTH = sizeof(struct DimPacket) - sizeof(char *);
#endif //DIM_DIMPROTOCOL_H
