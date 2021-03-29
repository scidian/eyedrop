//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_RANDOM_H
#define DR_RANDOM_H

// Forward Declarations
class DrColor;   
    
//####################################################################################
//##    Random Functions
//############################
int         RandomInt(int lower, int upper);                                        // Returns a number between lower (inclusive) and upper (exclusive)
double      RandomDouble(double lower, double upper);                               // Returns a number between lower (inclusive) and upper (inclusive
bool        RandomBool();                                                           // Returns a random boolean
DrColor     RandomColor();                                                          // Returns a random DrColor, fully opaque

#endif // DR_RANDOM_H
