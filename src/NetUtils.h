//
// Created by delinoz on 19-5-19.
//

#ifndef DIM_NETUTILS_H
#define DIM_NETUTILS_H

#include <cstring>
#include <vector>

using std::string;
using std::vector;

typedef struct NicInfo {
    char mac[16];
    string name;
} NicInfo;

typedef struct NicInfoSet {
    vector<NicInfo> infos;
    vector<string> ips;
    int nicNum;
} NicInfoSet;

class NetUtils {
public:
    static void getLocalHostMacAddress(NicInfoSet &nicInfoSet);

    static void getIpv4s(NicInfoSet &nicInfoSet);

    static struct sockaddr_in getSockaddr_in(const char *ip, int destPort);
};


#endif //DIM_NETUTILS_H
