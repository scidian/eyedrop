//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_RECTF_H
#define DR_RECTF_H

// Forward Declarations
class DrPoint;
class DrPointF;
class DrRect;


//####################################################################################
//##    DrRectF
//##        A double Rectangle Class, Y axis starts at 0 at top and increases as it goes downward
//############################
class DrRectF
{

public:
    double  x;                      // As vec4, x
    double  y;                      // As vec4, y
    double  width;                  // As vec4, z
    double  height;                 // As vec4, w

    // Constructors
    DrRectF();
    DrRectF(double x_, double y_, double width_, double height_);
    DrRectF(const DrPointF &top_left, const DrPointF &bottom_right);
    DrRectF(const DrRect  &r);
    DrRectF(const DrRectF &r);
    
    // Conversion
    DrRect          toRect();
    
    // Helper Functions
    bool            contains(const DrPoint);
    bool            contains(const DrPointF);

    // Getters
    double          left();
    double          right();
    double          top();
    double          bottom();

    DrPointF        topLeft();
    DrPointF        topRight();
    DrPointF        bottomLeft();
    DrPointF        bottomRight();

};


#endif // DR_RECTF_H






