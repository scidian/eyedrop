//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_POINT_H
#define DR_POINT_H

#include <vector>

// Forward Declarations
class DrPointF;


//####################################################################################
//##    DrPoint
//##        Useful 2D int Point Class, adapted from HullFinder
//############################
class DrPoint
{
public:
    int x;
    int y;

public:
    // Constructors
    DrPoint();
    DrPoint(int x, int y_);
    DrPoint(long x, long y_);
    DrPoint(float x_, float y_);
    DrPoint(double x_, double y_);
    DrPoint(const DrPointF pointf);

    // Operator Overloads
    DrPoint&    operator=   (const DrPoint& other);
    DrPoint     operator+   (const DrPoint& other) const;
    DrPoint     operator-   (const DrPoint& other) const;
    DrPoint     operator*   (int k) const;
    DrPoint     operator/   (int k) const;
    bool        operator==  (const DrPoint& other) const;

    // Conversions
    DrPointF toPointF();
};

#endif // DR_POINT_H

