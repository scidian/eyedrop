//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Tue Mar 09 2021
//
//
#ifndef DR_DATA_H
#define DR_DATA_H

#include <string>
#include <vector>


//####################################################################################
//##    DrData Types
//############################
enum class Data_Type {
    Bool            =   1,
    Int             =   2,
    Long            =   3,
    Unsigned_Int    =   4,
    Float           =   5,
    Double          =   6,
    String          =   7,
    Unknown         =   1001,
};


//####################################################################################
//##    DrData
//##        Template class to hold one value of a DrProperty
//############################
template<typename T> 
class DrData
{
private:
    // Variables
    T           m_value;                                                            // Value of this Data
    Data_Type   m_value_type;                                                       // Keeping track of what type of Data this is

public:
    // Constructors
    DrData(const T& value) : m_value(value) { }    
    DrData(bool value)                          { m_value = value;      m_value_type = Data_Type::Bool; }
    ~DrData() { }

    // DrVariant(int value)                    { m_value = value;      m_type = Variant_Type::Int; }
    // DrVariant(long value)                   { m_value = value;      m_type = Variant_Type::Long; }
    // DrVariant(unsigned int value)           { m_value = value;      m_type = Variant_Type::Unsigned_Int; }
    // DrVariant(float value)                  { m_value = value;      m_type = Variant_Type::Float; }
    // DrVariant(double value)                 { m_value = value;      m_type = Variant_Type::Double; }
    // DrVariant(std::string value)            { m_value = value;      m_type = Variant_Type::String; }
    // DrVariant(DrPoint value)                { m_value = value;      m_type = Variant_Type::Point; }
    // DrVariant(DrPointF value)               { m_value = value;      m_type = Variant_Type::PointF; }
    // DrVariant(DrVec2 value)                 { m_value = value;      m_type = Variant_Type::Vec2; }
    // DrVariant(DrVec3 value)                 { m_value = value;      m_type = Variant_Type::Vec3; }
    // DrVariant(std::vector<DrVariant> value) { m_value = value;      m_type = Variant_Type::Vector; }
    // DrVariant()                             { m_value = int(0);     m_type = Variant_Type::Int; }
    // template<class T> DrVariant(T value)    { m_value = value;      m_type = Variant_Type::Unknown; }
};


#endif  // DR_DATA_H

