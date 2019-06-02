//
// Created by delinoz on 19-5-19.
//

#ifndef DIM_ARRAYS_H
#define DIM_ARRAYS_H

#define ARRAY_LENGTH(arr) sizeof(arr)/sizeof(arr[0])

class Arrays {
public:
//    template <typename T>
//    static int length(T&);
    static int length(char arr[]);
};


#endif //DIM_ARRAYS_H
