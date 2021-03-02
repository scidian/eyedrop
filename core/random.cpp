//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include <chrono>

#include "random.h"
#include "types/color.h"


namespace Dr {

//####################################################################################
//##
//##    Random Functions
//##
//####################################################################################
// Returns a number between lower (inclusive) and upper (exclusive)
int RandomInt(int lower, int upper) {
    static unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    static std::default_random_engine generator (seed);

    std::uniform_int_distribution<int> distribution(lower, upper - 1);
    return distribution(generator);
}

// Returns a number between lower (inclusive) and upper (inclusive)
double RandomDouble(double lower, double upper) {
    static unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    static std::default_random_engine generator (seed);

    std::uniform_int_distribution<double> distribution(lower * 10000.0, upper * 10000.0);
    return (distribution(generator) / 10000.0);
}

// Returns a random true / false
bool RandomBool() {
    static unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    static std::default_random_engine generator (seed);

    std::uniform_int_distribution<int> distribution(1, 2);
    return (distribution(generator) == 1);
}

// Returns a random color with fully opaque alpha
DrColor RandomColor() {
    DrColor color(0, 0, 0, 255);
    color.setRed(   RandomInt(0, 256) );
    color.setGreen( RandomInt(0, 256) );
    color.setBlue(  RandomInt(0, 256) );
    return color;
}


}   // End namespace Dr



