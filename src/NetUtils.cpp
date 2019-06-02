//
// Created by delinoz on 19-5-19.
//

#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "NetUtils.h"

/**
 * 获得本机的MAC
 * @param nicInfoSet 接受主机网卡信息
 */
void NetUtils::getLocalHostMacAddress(NicInfoSet &nicInfoSet) {
    using std::stringstream;
    using std::cout;
    using std::endl;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("open socket error!");
        exit(1);
    }
    struct ifconf ifc;
    struct ifreq buf[16];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc)) {
        int interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
        nicInfoSet.nicNum = interfaceNum;
        while (interfaceNum-- > 0) {
            if (!ioctl(fd, SIOCGIFHWADDR, (char *) (&buf[interfaceNum]))) {
                NicInfo info;
                char mac[16];
                memset(mac, 0, sizeof(mac));
                snprintf(mac, sizeof(mac), "%02x%02x%02x%02x%02x%02x",
                         (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[0],
                         (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[1],
                         (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[2],
                         (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[3],
                         (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[4],
                         (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[5]);
                strcpy(info.mac, mac);
                stringstream ss;
                ss << "nic-" << interfaceNum;
                cout << ss.str() << endl;
                info.name = ss.str();
                nicInfoSet.infos.push_back(info);
            }
            struct sockaddr_in *sin = (struct sockaddr_in *) (&buf[interfaceNum].ifr_addr);
            if (sin->sin_family == AF_INET) {
                cout << buf[interfaceNum].ifr_ifrn.ifrn_name << endl;
                cout << inet_ntoa(sin->sin_addr) << endl;
            }
        }
    }
    close(fd);
}
/**
 * 获取IPv4地址
 * @param nicInfoSet
 */
void NetUtils::getIpv4s(NicInfoSet &nicInfoSet) {
    struct ifaddrs *ifAddrStruct = NULL;
    getifaddrs(&ifAddrStruct);
    while (ifAddrStruct != nullptr) {
        if (ifAddrStruct->ifa_addr->sa_family == AF_INET) {
            void *tmpAddrPtr = &((struct sockaddr_in *) ifAddrStruct->ifa_addr)->sin_addr;
            char ipaddr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, ipaddr, INET_ADDRSTRLEN);
            nicInfoSet.ips.push_back(ipaddr);
        }
        ifAddrStruct = ifAddrStruct->ifa_next;
    }
}

struct sockaddr_in NetUtils::getSockaddr_in(const char *ip, int destPort) {
    struct sockaddr_in sockaddrIn;
    memset(&sockaddrIn, 0, sizeof(struct sockaddr_in));
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(destPort);
    int ret;
    //转换字符串IP
    if (ret=inet_aton(ip, &sockaddrIn.sin_addr)<0){
        perror("ip trans error");
    }
    return sockaddrIn;
}
