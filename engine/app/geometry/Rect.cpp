/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "../core/Math.h"
#include "Point.h"
#include "PointF.h"
#include "Rect.h"
#include "RectF.h"


//####################################################################################
//##    Constructors
//####################################################################################
DrRect::DrRect() {
    x =         0;
    y =         0;
    width =     0;
    height =    0;
}
DrRect::DrRect(int x_, int y_, int width_, int height_) {
    x =         x_;
    y =         y_;
    width =     width_;
    height =    height_;
}
DrRect::DrRect(const DrPoint& top_left, const DrPoint& bottom_right) {
    x =         top_left.x;
    y =         top_left.y;
    width =     (bottom_right.x - top_left.x);
    height =    (bottom_right.y - top_left.y);
}
DrRect::DrRect(const DrRect& r) {
    x =         r.x;
    y =         r.y;
    width =     r.width;
    height =    r.height;
}
DrRect::DrRect(const DrRectF& r) {
    x =         static_cast<int>(r.x);
    y =         static_cast<int>(r.y);
    width =     static_cast<int>(r.width);
    height =    static_cast<int>(r.height);
}
DrRect::DrRect(const hmm_vec4& v) {
    x =         static_cast<int>(v.X);
    y =         static_cast<int>(v.Y);
    width =     static_cast<int>(v.Z);
    height =    static_cast<int>(v.W);
}


//####################################################################################
//##    Conversion
//####################################################################################
DrRectF DrRect::toRectF()               { return DrRectF(*this); }


//####################################################################################
//##    Helper Functions
//####################################################################################
bool DrRect::contains(const DrPointF pointf) { return contains(DrPoint(pointf)); }
bool DrRect::contains(const DrPoint point) {
    if (point.x > left() && point.x < right()) {
        if (point.y > top() && point.y < bottom())
            return true;
    }
    return false;
}

// Adjusts bounds of Rect
void DrRect::adjust(int adjust_left, int adjust_top, int adjust_right, int adjust_bottom) {
    x += adjust_left;   width  += (adjust_left * -1);
    y += adjust_top;    height += (adjust_top  * -1);
    width  += adjust_right;
    height += adjust_bottom;
}


//####################################################################################
//##    Getters
//####################################################################################
int     DrRect::left()                  { return (width > 0) ? (x) : (x + width + 1); }
int     DrRect::right()                 { return (width > 0) ? (x + width - 1) : (x); }

int     DrRect::top()                   { return (height > 0) ? (y) : (y + height + 1); }
int     DrRect::bottom()                { return (height > 0) ? (y + height - 1) : (y); }

DrPoint DrRect::topLeft()               { return DrPoint(left(),    top()); }
DrPoint DrRect::topRight()              { return DrPoint(right(),   top()); }
DrPoint DrRect::bottomLeft()            { return DrPoint(left(),    bottom()); }
DrPoint DrRect::bottomRight()           { return DrPoint(right(),   bottom()); }

DrPoint DrRect::mid()                   { return DrPoint(left() + (width * 0.5), top() + (height * 0.5)); }



