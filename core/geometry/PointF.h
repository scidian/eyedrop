//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_POINT_F_H
#define DR_POINT_F_H

#include <vector>

// Forward Declarations
class DrPoint;


//####################################################################################
//##    DrPointF
//##        Useful 2D double Point Class, adapted from HullFinder
//############################
class DrPointF
{
public:
    double x;
    double y;

public:
    // Constructor
    DrPointF();
    DrPointF(double x_, double y_);
    DrPointF(const DrPoint point);

    // Operator Overloads
    DrPointF&   operator=   (const DrPointF& other);
    DrPointF    operator+   (const DrPointF& other) const;
    DrPointF    operator-   (const DrPointF& other) const;
    DrPointF    operator*   (double k) const;
    DrPointF    operator/   (double k) const;
    DrPointF&   operator*=  (double k);
    bool        operator==  (const DrPointF& other) const;

    // Functions
    double      dotProduct(const DrPointF& other) const;
    double      distanceSquared(const DrPointF& to) const;
    double      distance(const DrPointF& to) const;
    double      distance(const DrPointF& segment_start, const DrPointF& segment_end) const;
    double      decisionDistance(const std::vector<DrPointF>& points) const;

    // Conversions
    DrPoint     toPoint();
};




#endif // DR_POINT_F_H






