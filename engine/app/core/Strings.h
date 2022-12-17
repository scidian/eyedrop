/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef SCID_STRINGS_H
#define SCID_STRINGS_H

#include <math.h>
#include <string>


//####################################################################################
//##    Decimal Strings
//############################
// Returns number as string with decimal_places precision
template<typename T> std::string RoundToDecimalPlace(const T& number, const int decimal_places) {
    double num_as_double = static_cast<double>(number);
    if (decimal_places <= 0) {
        std::string s = std::to_string(num_as_double);
        return s.substr(0, s.find('.'));
    } else {
        num_as_double = round(num_as_double * pow(10.0, decimal_places)) / pow(10.0, decimal_places);
        std::string s = std::to_string(num_as_double);
        return s.substr(0, s.find('.') + 1 + decimal_places);
    }
}

std::string RemoveTrailingZeros(const std::string& source);                         // Removes trailing zeros from a number, returns as string


//####################################################################################
//##    Basic Functions
//############################
std::string Left(const std::string& source, const size_t length);                   // Returns (length) number of characters from the left side of a string
std::string Right(const std::string& source, const size_t length);                  // Returns (length) number of characters from the right side of a string

bool        IsInteger(const std::string& source);                                   // Returns true if string is a positive integer, otherwise false

std::string HexDigit(const int integer);                                            // Returns integer <=  15 as hex string
std::string HexString(const int integer);                                           // Returns integer <= 255 as hex string


//####################################################################################
//##    String Formatting
//############################
void        CreateNiceTitle(std::string& name);                                     // Creates nice title from filename (spaces, words capitalized)
void        FileNameOnly(std::string& filename);                                    // Gets 'filename' from "/usr/local/filename.png"
void        LowerCase(std::string& str);                                            // Makes all characters lowercase
void        RemoveUnderscores(std::string& str);                                    // Removes underscores from string


#endif // SCID_STRINGS_H

