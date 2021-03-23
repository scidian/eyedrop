//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include <ctype.h>
#include "core/Strings.h"


//####################################################################################
//##
//##    String Functions
//##
//####################################################################################
// Removes trailing zeros from a number, returns as string
std::string RemoveTrailingZeros(const std::string& source) {
    std::string str = source;
    int offset{1};
    if (str.find_last_not_of('0') == str.find('.')) {
        offset = 0;
    }
    str.erase(str.find_last_not_of('0') + offset, std::string::npos);
    return str;
}

// Returns (length) number of characters from the left side of a string
std::string Left(const std::string& source, const size_t length) {
    if (length >= source.size()) { return source; }
    return source.substr(0, length);
}

// Returns (length) number of characters from the right side of a string
std::string Right(const std::string& source, const size_t length) {
    if (length >= source.size()) { return source; }
    return source.substr(source.size() - length);
}

// Returns true if string is a positive integer, otherwise false
bool IsInteger(const std::string& source) {
    return !source.empty() && std::find_if(source.begin(), source.end(), [](char c) { return !std::isdigit(c); }) == source.end();
}

// Returns integer as hex string
std::string HexString(const int integer) {
    std::stringstream ss;
    ss << std::hex << static_cast<int>(integer);
    return ss.str();
}

