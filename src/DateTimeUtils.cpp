//
// Created by delinoz on 19-5-19.
//
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "DateTimeUtils.h"

int64_t DateTimeUtils::getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);    //该函数在sys/time.h头文件中
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
//函数内部定义的数组生命期不定
char *DateTimeUtils::formatTime(int64_t timestamp, char *timeFmt, int bufferSize) {
    time_t ts = timestamp;
    struct tm *timeinfo;
    time(&ts);
    timeinfo = localtime(&ts);
    strftime(timeFmt, bufferSize, "%Y-%m-%d %H:%M:%S", timeinfo);
    return timeFmt;
}