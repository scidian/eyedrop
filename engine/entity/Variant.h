//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_VARIANT_H
#define DR_VARIANT_H

#include <deque>
#include <list>
#include <vector>

#include "3rd_party/any.h"
#include "core/geometry/Point.h"
#include "core/geometry/PointF.h"
#include "core/geometry/PolygonF.h"
#include "core/geometry/Rect.h"
#include "core/geometry/RectF.h"
#include "core/geometry/Vec2.h"
#include "core/geometry/Vec3.h"
#include "core/imaging/Bitmap.h"
#include "core/imaging/Color.h"


//####################################################################################
//##    DrVariant Types
//############################
enum class Variant_Type {
    Bool            =   1,
    Int             =   2,
    Long            =   3,
    Unsigned_Int    =   4,
    Float           =   5,
    Double          =   6,
    String          =   7,
    Point           =   8,
    PointF          =   9,
    Vec2            =  10,
    Vec3            =  11,
    Vector          =  12,
    Unknown         = 100,
};


//####################################################################################
//##    DrVariant
//##        Holds anything, mostly wrapper for boost::any
//############################
class DrVariant
{
public:
    nonstd::any     m_value;                                        // Holds the variant value
    Variant_Type    m_type;                                         // Holds the variant type, if known

public:
    // Constructors
    DrVariant(bool value)                   { m_value = value;      m_type = Variant_Type::Bool; }
    DrVariant(int value)                    { m_value = value;      m_type = Variant_Type::Int; }
    DrVariant(long value)                   { m_value = value;      m_type = Variant_Type::Long; }
    DrVariant(unsigned int value)           { m_value = value;      m_type = Variant_Type::Unsigned_Int; }
    DrVariant(float value)                  { m_value = value;      m_type = Variant_Type::Float; }
    DrVariant(double value)                 { m_value = value;      m_type = Variant_Type::Double; }
    DrVariant(std::string value)            { m_value = value;      m_type = Variant_Type::String; }
    DrVariant(DrPoint value)                { m_value = value;      m_type = Variant_Type::Point; }
    DrVariant(DrPointF value)               { m_value = value;      m_type = Variant_Type::PointF; }
    DrVariant(DrVec2 value)                 { m_value = value;      m_type = Variant_Type::Vec2; }
    DrVariant(DrVec3 value)                 { m_value = value;      m_type = Variant_Type::Vec3; }
    DrVariant(std::vector<DrVariant> value) { m_value = value;      m_type = Variant_Type::Vector; }
    DrVariant()                             { m_value = int(0);     m_type = Variant_Type::Int; }
    template<class T> DrVariant(T value)    { m_value = value;      m_type = Variant_Type::Unknown; }

    // Operator Overloads
    DrVariant&      operator=   (const DrVariant &other);
    bool            operator==  (DrVariant &other);

    // Getters
    std::string     getRawType()        { return m_value.type().name(); }
    Variant_Type    getType() const     { return m_type; }
    nonstd::any     value() const       { return m_value; }

    // Returns
    bool                    toBool();
    double                  toDouble();
    float                   toFloat();
    int                     toInt();
    long                    toLong();
    unsigned int            toUInt();

    std::string             toString();

    std::deque<DrVariant>   toDeque();
    std::list<DrVariant>    toList();
    std::vector<DrVariant>  toVector();

    DrColor                 toColor();
    DrBitmap                toImage();
    DrPoint                 toPoint();
    DrPointF                toPointF();
    DrPolygonF              toPolygonF();
    DrRect                  toRect();
    DrRectF                 toRectF();
    DrVec2                  toVec2();
    DrVec3                  toVec3();

};


#endif // DR_VARIANT_H






