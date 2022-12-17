/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
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
    DrRectF(const DrPointF& top_left, const DrPointF& bottom_right);
    DrRectF(const DrRect& r);
    DrRectF(const DrRectF& r);

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

    DrPointF        mid();
};


#endif // DR_RECTF_H






