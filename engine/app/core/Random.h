/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
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
