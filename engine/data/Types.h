//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_ENGINE_DATA_TYPES_H
#define DR_ENGINE_DATA_TYPES_H

#include <bitset>
#include <cstdint>

// Global Enum Constants
#define DROP_VERSION        2000                            // Drop Creator version number, important for file loading / saving through Cereal

#define KEY_NONE              -1                            // Value that represents no item selected
#define KEY_START              1                            // Starting value for key generators

#define MAX_ENTITIES       10000                            // Total of number of entities allowed for now
#define MAX_COMPONENTS        32                            // Current maximum number of compoenents (uint_8), used for sizing Signature

// ECS System
using DrEntity =            std::uint32_t;                  // Unique Entity ID Key
using DrComponentType =     std::uint8_t;                   // Identifier for Components
using DrArchetype =         std::bitset<MAX_COMPONENTS>;    // A bitset to represent a collection of Components (formerly Signature)


//####################################################################################
//##    Used to track what types Properties of Components are, stored in DrReflect meta data
//##    !!!!! Numbers are SAVE FILE CRITICAL and CANNOT BE CHANGED once set
//##
//##    !!!!! NEXT AVAILABLE NUMBER:    35
//##
//############################
enum class Property_Type {
    Unknown         =  0,   // unknown!!            Uh-oh!

    Bool            =  1,   // bool                 True or False                                                   (false == 0, true != 0)
    BoolInt         =  2,   // vector<int>          True or False, when True shows int spinbox                      sizeof 6 - bool, value, min, max, step size
    BoolDouble      =  3,   // vector<double>       True or False, when True shows double spinbox                   sizeof 6 - bool, value, min, max, step size
    BoolColor       =  4,   // vector<uint>         True or False, when True shows color                            sizeof 2 - bool, color (as unsigned int)

    Int             =  5,   // int                  Integer
    IntPositive     =  6,   // int                  Integer >= 0
    IntRanged       =  7,   // vector<int>          Integer >= min and <= max                                       sizeof 4 - value, min, max, step size
    IntSlider       =  8,   // vector<int>          Decimal >= min and <= max, with a slider                        sizeof 4 - value, min, max, step size

    Double          =  9,   // double               Decimal
    DoublePositive  = 10,   // double               Decimal >= 0.0
    DoubleRanged    = 11,   // vector<double>       Decimal >= min and <= max                                       sizeof 4 - value, min, max, step size
    DoubleSlider    = 12,   // vector<double>       Decimal >= min and <= max, with a slider                        sizeof 4 - value, min, max, step size
    DoubleAngle     = 13,   // double               Decimal for showing degrees, shows degree symbol in spinbox
    DoublePercent   = 14,   // double               Decimal from 0.0 to 100.0, shows percent symbol with a slider

    Point           = 17,   // vector<int>          Integer pair x and y                                            sizeof 2 - x, y
    PointF          = 17,   // vector<double>       Decimal pair x and y                                            sizeof 2 - x, y
    Point3D         = 24,   // vector<double>       Decimal point trio, x, y, z                                     sizeof 3 - doubles: x, y, z
    Variable2D      = 25,   // vector<double>       Decimal point pair, number followed by a +/- number             sizeof 3 - doubles: x, y, variation
    Variable3D      = 26,   // vector<double>       Decimal point trio, number followed by a +/- number             sizeof 4 - doubles: x, y, z, variation

    PositionF       = 18,   // vector<double>       Decimal pair x and y, used for object positions in scene        sizeof 2 - x, y
    ScaleF          = 19,   // vector<double>       Decimal pair x and y, has smaller increment step in spin box    sizeof 2 - x, y
    PositiveScaleF  = 20,   // vector<double>       Decimal pair x and y, both numbers are >= 0.0                   sizeof 2 - x, y
    SizeF           = 21,   // vector<double>       Decimal pair w and h                                            sizeof 2 - w, h
    PositiveSizeF   = 22,   // vector<double>       Decimal pair w and h, both numbers are >= 0.0                   sizeof 2 - w, h   

    String          = 15,   // string               Uses LineEdit
    Textbox         = 16,   // string               Uses TextEdit for multi-line text input

    List            = 27,   // int                  Index of a particular enum list
    ArrayInt        = 28,   // vector<int>          Array of ints
    ArrayDouble     = 29,   // vector<double>       Array of double
    ArrayString     = 30,   // vector<string>       Array of strings

    Color           = 31,   // unsigned int         For colors (compatible with DrColor.rgba())
    Image           = 32,   // int                  Index of a particular DrAnimation Asset within the Project

    // Future: 
    Collision       = 33,   // ?????                Maybe vector<points> (for Collision Shapes)
    Icon            = 34,   // ?????                Maybe string
};


//####################################################################################
//##
//##    Main Types of Entities possible in Project
//##        - All Entities inherit DrEntity which contain DrComponents, which are displayed in the Inspector
//##
//############################
enum Archetype {    
    // Misc Types
    NotFound = 0,                   // For passing a value in functions that represents No Type Selected / Found, !!!!! #NOTE: Keep as zero

    // Shared Types
    Prefab,                         // Keeps information about an Object so we can easily make a copy
    Image,                          // Images for use in Project

    // Game Types
    World,                          // Describes a World (physics, lighting, etc), also holds a colleciton of Scenes
    Scene,                          // Holds a collection of Objects
    Object,                         // Objects contained within Scene

};


// ################## Project Mapped Types ####################
enum class Scene_Type {
    Physics_2D,
    Physics_3D,
    UI,
};


enum Component_Icon {
    None,
    Toolbar_World_Graph,
};


#endif  // DR_ENGINE_DATA_TYPES_H

