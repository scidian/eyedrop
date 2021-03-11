//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_STRINGS_H
#define DR_STRINGS_H

#include <algorithm>
#include <sstream>
#include <string>


//####################################################################################
//##    Decimal Strings
//############################
/// @brief: Returns number as string with decimal_places precision
template<typename T> std::string RoundToDecimalPlace(const T& number, const int decimal_places) {
    double num_as_double = static_cast<double>(number);
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(decimal_places);
    ss << num_as_double;
    return ss.str();
}

/// @brief: Removes trailing zeros from a number, returns as string
std::string RemoveTrailingZeros(const std::string& source);


//####################################################################################
//##    String Functions
//############################
/// @brief: Returns (length) number of characters from the left side of a string
std::string Left(const std::string& source, const size_t length);

/// @brief: Returns (length) number of characters from the right side of a string
std::string Right(const std::string& source, const size_t length);

/// @brief: Returns true if string is a positive integer, otherwise false
bool        IsInteger(const std::string& source);

/// @brief: Returns integer as hex string
std::string HexString(const int integer);


#endif // DR_STRINGS_H

