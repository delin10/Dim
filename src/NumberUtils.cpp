//
// Created by delinoz on 19-5-19.
//

#include <cmath>
#include <iostream>
#include "NumberUtils.h"

using namespace std;

/**
 * 十六进制转long
 * @param hex
 * @param len
 * @return
 */
long NumberUtils::hexToLong(const char *hex, size_t len) {
    long result = 0;
    for (int i = 0; i < len; ++i) {
        long cur = 0;
        char digit = hex[i];
        if ('a' <= digit && digit <= 'f') {
            cur = 10 + digit - 'a';
        } else if ('A' <= digit && digit <= 'F') {
            cur = 10 + digit - 'A';
        } else if ('0' <= digit && digit <= '9') {
            cur = digit - '0';
        }
        result += cur * pow(16, len - i - 1);
    }
    return result;
}

/**
 * 十进制转16进制
 * @param number
 * @param buf
 * @param size
 */
void NumberUtils::longToHex(long number, char *buf, size_t size) {
    long quot = number;
    int buff[size];
    int digit = 0;
    while (quot != 0) {
        buff[digit++] = quot & 0xF;
        quot = quot >> 4;
    }
    int totalDigit = digit;
    while (digit-- > 0) {
        int cur = buff[totalDigit - digit - 1];
        buf[digit] = cur < 10 ? cur + '0' : cur - 10 + 'a';
    }
    buf[totalDigit] = '\0';
}