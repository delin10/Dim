//
// Created by delinoz on 19-5-22.
//
#include "debug.h"
#include "StringUtils.h"

void StringUtils::regexSplit(string &input, vector<string> &container, string seperator) {
    std::regex re(seperator);
    std::sregex_token_iterator p(input.begin(), input.end(), re, -1);
    std::sregex_token_iterator end;
    while (p != end) {
        container.emplace_back(*p++);
    }
}

int StringUtils::toInt(string str) {
    std::stringstream ss;
    ss << str;
    int intValue;
    ss >> intValue;
    return intValue;
}

long StringUtils::toLong(string str) {
    std::stringstream ss;
    ss << str;
    long longValue;
    ss >> longValue;
    return longValue;
}