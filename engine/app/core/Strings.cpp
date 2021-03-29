//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include <ctype.h>
#include "engine/app/core/Strings.h"


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

// Returns integer <= 15 as hex string
std::string HexDigit(const int integer) {
    switch (integer) {
        case  0:    return "0";
        case  1:    return "1";
        case  2:    return "2";
        case  3:    return "3";
        case  4:    return "4";
        case  5:    return "5";
        case  6:    return "6";
        case  7:    return "7";
        case  8:    return "8";
        case  9:    return "9";
        case 10:    return "a";
        case 11:    return "b";
        case 12:    return "c";
        case 13:    return "d";
        case 14:    return "e";
        case 15:    return "f";
        default:    return "?";
    }
}

// Returns integer <= 255 as hex string
std::string HexString(const int integer) {
    std::string result = "";
    int start_number = integer;
    if (start_number > 15) {
        int twos = start_number / 16;
        result += HexDigit(twos);
        start_number -= (twos * 16);
    } else {
        result += "0";
    }
    result += HexDigit(start_number);
    return result;
}

