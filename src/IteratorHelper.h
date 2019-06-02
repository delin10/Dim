//
// Created by delinoz on 19-5-29.
//

#ifndef DIM_ITERATORHELPER_H
#define DIM_ITERATORHELPER_H

#include <vector>
#include <string>

#define DIM_ITERATORHELPER_H_DEBUG
#ifdef DIM_ITERATORHELPER_H_DEBUG

#include <iostream>

using std::cout;
using std::endl;
#endif


using std::string;
using std::iterator;
using std::vector;

class IteratorHelper {
public:
    static bool pop(vector<string> &collection, vector<string>::iterator &iterator, string *str);
};


#endif //DIM_ITERATORHELPER_H
