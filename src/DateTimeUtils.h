//
// Created by delinoz on 19-5-19.
//

#ifndef DIM_DATETIMEUTILS_H
#define DIM_DATETIMEUTILS_H


#include <cstdint>

class DateTimeUtils {
public:
    static int64_t getCurrentTime();

    static char *formatTime(int64_t timestamp, char *buffer, int bufferSize);
};


#endif //DIM_DATETIMEUTILS_H
