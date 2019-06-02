//
// Created by delinoz on 19-5-19.
//

#include "Arrays.h"

//退化成了指针
//sizeof(ptr)返回指针长度
//template <typename T>
//int Arrays::length(T& arr) {
//    return sizeof(arr)/ sizeof(arr[0]);
//}

int Arrays::length(char arr[]) {
    return sizeof(arr) / sizeof(arr[0]);
}