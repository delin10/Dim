//
// Created by delinoz on 19-5-22.
//

#ifndef DIM_STRINGUTILS_H
#define DIM_STRINGUTILS_H

#include <string>
#include <vector>
#include <regex>

using std::string;
using std::vector;

class StringUtils {
public:
    static void regexSplit(string &input, vector<string> &container, string regex);

    static int toInt(string str);

    static long toLong(string str);
};


#endif //DIM_STRINGUTILS_H
