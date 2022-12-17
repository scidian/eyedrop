/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include <ctype.h>
#include "engine/app/core/Strings.h"


//####################################################################################
//##    Decimal Strings
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


//####################################################################################
//##    Basic Functions
//####################################################################################
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


//####################################################################################
//##    String Formatting
//####################################################################################
void CreateNiceTitle(std::string& name) {
    // Removes underscores
    RemoveUnderscores(name);

    // Capitalize words
    name[0] = toupper(name[0]);
    for (int c = 1; c < name.length(); c++) {
        if (name[c - 1] == ' ') name[c] = toupper(name[c]);
    }

    // Add spaces to seperate words if they're not yet seperated
    std::string title = "";
    title += name[0];
    for (int c = 1; c < name.length(); c++) {
        if (islower(name[c - 1]) && isupper(name[c])) {
            title += std::string(" ");
        } else if ((isalpha(name[c - 1]) && isdigit(name[c]))) {
            title += std::string(" ");
        }
        title += name[c];
    }
    name = title;
}

void FileNameOnly(std::string& filename) {
    // Remove directories containing backslash, '\'
    size_t last_backslash_index = filename.find_last_of("\\");
    if (std::string::npos != last_backslash_index) filename.erase(0, last_backslash_index + 1);

    // Remove directories containing '/'
    size_t last_slash_index = filename.find_last_of("/");
    if (std::string::npos != last_slash_index) filename.erase(0, last_slash_index + 1);

    // Remove file extension
    size_t period_index = filename.rfind('.');
    if (std::string::npos != period_index) filename.erase(period_index);
}

void LowerCase(std::string& str) {
    for (int c = 0; c < str.length(); c++) {
        str[c] = tolower(str[c]);
    }
}

void RemoveUnderscores(std::string& str) {
    std::replace(str.begin(), str.end(), '_', ' ');
}


