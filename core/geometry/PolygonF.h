//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_POLYGONF_H
#define DR_POLYGONF_H

#include <vector>

// Forward Declarations
class DrPointF;

// Enums
enum Winding_Orientation {
    DROP_WINDING_CLOCKWISE           = 0,
    DROP_WINDING_COUNTERCLOCKWISE    = 1,
    DROP_WINDING_LINE_SEGMENT        = 2,
};


//####################################################################################
//##    DrPolygonF
//##        A polygon class, useful for intersections
//############################
class DrPolygonF
{
private:
    std::vector<DrPointF> m_points;

public:
    // Constructor
    DrPolygonF();
    DrPolygonF(const DrPolygonF &polygon);

    // Access
    const std::vector<DrPointF>& points()       { return m_points; }

    // Info
    bool            isEmpty() const             { return (m_points.size() == 0); }
    long            numberOfPoints() const      { return static_cast<long>(m_points.size()); }

    // Manangement
    void            addPoint(DrPointF point);
    void            clear()                     { m_points.clear(); }

    // Polygon / Line Functions
    static bool                 onSegment(DrPointF line_a, DrPointF point, DrPointF line_b);
    static Winding_Orientation  orientation(DrPointF p, DrPointF q, DrPointF r);
    static void                 ensureWindingOrientation(std::vector<DrPointF> &points, Winding_Orientation direction_desired);
    static Winding_Orientation  findWindingOrientation(const std::vector<DrPointF> &points);
    static bool                 doIntersect(DrPointF p1, DrPointF q1, DrPointF p2, DrPointF q2);
    bool                        isInside(DrPointF point);



};

#endif // DR_POLYGONF_H





