//
// Created by delinoz on 19-5-19.
//

#ifndef DIM_NUMBERUTILS_H
#define DIM_NUMBERUTILS_H


class NumberUtils {
public:
    static long hexToLong(const char *hex, size_t len);

    static void longToHex(long number, char *buf, size_t size);
};


#endif //DIM_NUMBERUTILS_H
