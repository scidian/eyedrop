/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "Point.h"
#include "PointF.h"


//####################################################################################
//##    Constructors
//####################################################################################
DrPoint::DrPoint() {
    x = 0;
    y = 0;
}

DrPoint::DrPoint(int x_, int y_) {
    x = x_;
    y = y_;
}

DrPoint::DrPoint(long x_, long y_) {
    x = static_cast<int>(x_);
    y = static_cast<int>(y_);
}

DrPoint::DrPoint(float x_, float y_) {
    x = static_cast<int>(x_);
    y = static_cast<int>(y_);
}

DrPoint::DrPoint(double x_, double y_) {
    x = static_cast<int>(x_);
    y = static_cast<int>(y_);
}

DrPoint::DrPoint(const DrPointF pointf) {
    x = static_cast<int>(pointf.x);
    y = static_cast<int>(pointf.y);
}

//####################################################################################
//##    Overload Operators
//####################################################################################
DrPoint& DrPoint::operator=(const DrPoint& other) {
    x = other.x;
    y = other.y;
    return *this;
}

DrPoint DrPoint::operator+(const DrPoint& other) const {
    return DrPoint(x + other.x, y + other.y);
}

DrPoint DrPoint::operator-(const DrPoint& other) const {
    return DrPoint(x - other.x, y - other.y);
}

DrPoint DrPoint::operator*(int k) const {
    return DrPoint(x * k, y * k);
}

DrPoint DrPoint::operator/(int k) const {
    return DrPoint(x / k, y / k);
}

bool DrPoint::operator==(const DrPoint& other) const {
    return (x == other.x) && (y == other.y);
}


//####################################################################################
//##    Conversions
//####################################################################################
DrPointF DrPoint::toPointF() {
    return DrPointF( x, y );
}





