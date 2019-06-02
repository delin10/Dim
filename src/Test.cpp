//
// Created by delinoz on 19-5-18.
//
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "UDPUtils.h"
#include "Arrays.h"
#include "DateTimeUtils.h"
#include "NetUtils.h"
#include "NumberUtils.h"
#include "DimClient.h"

using std::cout;
using std::endl;

int main_() {
    openServerSocket(10001);
    char arr[] = {1, 2, 3, 4};
    char buffer[128];
    printf("timestamp:%ld\n", DateTimeUtils::getCurrentTime());
    printf("%s\n", DateTimeUtils::formatTime(DateTimeUtils::getCurrentTime(), buffer, 128));
    NicInfoSet set;
    NetUtils::getLocalHostMacAddress(set);
    int num = set.nicNum;
    cout << "nic num:" << num << endl;
    for (int i = 0; i < num; ++i) {
        NicInfo info = set.infos[i];
        cout << i << endl;
        cout << info.name << "=" << info.mac << endl;
    }

    cout << NumberUtils::hexToLong("80fa5b3d6d7c", 12) << endl;
    char buf[12];
    NumberUtils::longToHex(141812760931708, buf, 12);
    cout << buf << endl;

    //string Test
    string str = "aaaaaa";
    char *str2Ptr0 = (char *) str.data();
    str2Ptr0[0] = 'b';
    cout << str2Ptr0 << "|" << str << endl;

    char *str2Ptr1 = (char *) str.c_str();
    str2Ptr0[0] = 'c';
    cout << str2Ptr1 << "|" << str << endl;

    char cc[] = {'a', 'b', '\0'};
    string str1 = cc;
    cc[1] = 'c';

    cout << cc << "|" << str1 << endl;
    long destHashId = NumberUtils::hexToLong("80fa5b3d6d7c", 12);
    openServerSocket(10002);
    DimClient client;
    DimTextMessage msg;
    char tt[] = "xxx";
    char *ttptr = tt;
    cout << "strlen:" << strlen(ttptr) << endl;
    msg.destHashId = destHashId;
    msg.text = tt;
    msg.type = PACKET_TYPE_TEXT;
    int res = client.sendText(msg);
    //Permission denied
    //可能防火墙 非
    //
    cout << "errno:" << strerror(errno) << endl;
    cout << "send res:" << res << endl;
    cout << "finished" << endl;
    return 1;
}