/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_RECT_H
#define DR_RECT_H

#include "3rd_party/handmade_math.h"

// Forward Declarations
class DrPoint;
class DrPointF;
class DrRectF;


//####################################################################################
//##    DrRect
//##        An int Rectangle Class, Y axis starts at 0 at top and increases as it goes downward
//############################
class DrRect
{

public:
    int     x;                      // As vec4, x
    int     y;                      // As vec4, y
    int     width;                  // As vec4, z
    int     height;                 // As vec4, w

    // Constructors
    DrRect();
    DrRect(int x_, int y_, int width_, int height_);
    DrRect(const DrPoint& top_left, const DrPoint& bottom_right);
    DrRect(const DrRect& r);
    DrRect(const DrRectF& r);
    DrRect(const hmm_vec4& v);

    // Conversion
    DrRectF         toRectF();

    // Operator Overloads
    DrRectF&        operator=   (const DrRectF& other);

    // Helper Functions
    bool            contains(const DrPoint);
    bool            contains(const DrPointF);
    void            adjust(int adjust_left, int adjust_top, int adjust_right, int adjust_bottom);

    // Getters
    int             left();
    int             right();
    int             top();
    int             bottom();

    DrPoint         topLeft();
    DrPoint         topRight();
    DrPoint         bottomLeft();
    DrPoint         bottomRight();

    DrPoint         mid();
};


#endif // DR_RECT_H







