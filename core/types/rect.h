//
// Copyright (C) 2021 Scididan Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_RECT_H
#define DR_RECT_H

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
    DrRect(const DrPoint &top_left, const DrPoint &bottom_right);
    DrRect(const DrRect  &r);
    DrRect(const DrRectF &r);

    // Conversion
    DrRectF         toRectF();

    // Operator Overloads
    DrRectF&        operator=   (const DrRectF &other);

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

};


#endif // DR_RECT_H







