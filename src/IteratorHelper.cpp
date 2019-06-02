//
// Created by delinoz on 19-5-29.
//

#include "IteratorHelper.h"

bool IteratorHelper::pop(vector<string> &collection, vector<string>::iterator &iterator, string *str) {
    if (iterator == collection.end()) {
        return false;
    }

    *str = *iterator;
    ++iterator;
    cout << "arg-1" << *str << endl;
    return true;
}