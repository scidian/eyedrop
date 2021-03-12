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

// STL Includes
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>

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
//##    Used to track what the DrVariant m_value data type really is
//##        Numbers are SAVE FILE CRITICAL and CANNOT BE CHANGED once set
//##
//##    !!!!! NEXT AVAILABLE NUMBER:    33
//##
//############################
enum class Property_Type {
    Unknown         =  0,   // unknown!!            Uh-oh!

    Bool            =  1,   // bool                 True or False (false == 0, true != 0)
    BoolInt         =  2,   // vector<int>          True or False, when True shows int spinbox,     sizeof 6 - bool, value, min, max, step size
    BoolDouble      =  3,   // vector<double>       True or False, when True shows double spinbox,  sizeof 6 - bool, value, min, max, step size
    BoolColor       =  4,   // vector<u_int>        True or False, when True shows color,           sizeof 2 - bool, color (as unsigned int)

    Int             =  5,   // long                 Integer
    PositiveInt     =  6,   // long                 Integer >= 0
    RangedInt       =  7,   // vector<variant>      Integer >= min and <= max,                      sizeof 4 - longs: value, min, max, step size

    Double          =  8,   // double               Decimal
    PositiveDouble  = 10,   // double               Decimal >= 0.0
    RangedDouble    = 11,   // vector<variant>      Decimal >= min and <= max,                      sizeof 4 - doubles: value, min, max, step size
    Slider          = 12,   // vector<variant>      Decimal >= min and <= max, with a slider        sizeof 5 - doubles: value, min, max, step size, string: suffix ("%", etc)
    Angle           =  9,   // double               Decimal for showing degrees, shows degree symbol in spinbox
    Percent         = 13,   // double               Decimal from 0.0 to 100.0, shows percent symbol, with a slider

    String          = 14,   // string               Uses LineEdit
    Textbox         = 15,   // string               Uses TextEdit for multi-line html text input

    PointF          = 16,   // DrPointF             Decimal pair x and y
    PositionF       = 17,   // DrPointF             Decimal pair x and y, used for object positions in scene
    ScaleF          = 18,   // DrPointF             Decimal pair x and y, has smaller increment step in spin box
    PositiveScaleF  = 19,   // DrPointF             Decimal pair x and y, both numbers are >= 0.0
    SizeF           = 20,   // DrPointF             Decimal pair w and h
    PositiveSizeF   = 21,   // DrPointF             Decimal pair w and h, both numbers are >= 0.0
    OneSizeF        = 22,   // DrPointF             Decimal pair w and h, both numbers are >= 1.0
    Point3D         = 23,   // DrVec3               Decimal point trio, x, y, z                     sizeof 3 - doubles: x, y, z

    GridF           = 24,   // DrPointF             Decimal pair x and y, minimum value of c_minimum_grid_size  for both x and y
    GridScaleF      = 25,   // DrPointF             Decimal pair x and y, minimum value of c_minimum_grid_scale for both x and y
    Variable        = 26,   // DrPointF             Decimal point pair, number followed by a +/- number

    List            = 27,   // long                 Index of a particular enum list
    Array           = 28,   // vector<variant>      List of DrVariants, some parts NOT IMPLEMENTED !!!!!

    Color           = 29,   // unsigned int         For colors (compatible with DrColor.rgba())
    Image           = 30,   // long                 Index of a particular DrAnimation within the Project

    // Future: 
    Collision       = 31,   // ?????                For Collision Shapes
    Icon            = 32,   // ?????                
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
enum class SceneType {
    Physics_2D,
    Physics_3D,
    UI,
};


// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count) {
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count) {
	return fnv1a_32(s, count);
}

// Input
enum class InputButtons
{
	W,
	A,
	S,
	D,
	Q,
	E
};


// Events
using EventId = std::uint32_t;
using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

// TODO: Make these easier to define and use (macro?)
// TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match

namespace Events_Window {
    const EventId QUIT =        "Events::Window::QUIT"_hash;
    const EventId RESIZED =     "Events::Window::RESIZED"_hash;
    const EventId INPUT =       "Events::Window::INPUT"_hash;
}

namespace Events_Window_Input {
    const ParamId INPUT =       "Events::Window::Input::INPUT"_hash;
}

namespace Events_Window_Resized {
    const ParamId WIDTH =       "Events::Window::Resized::WIDTH"_hash;
    const ParamId HEIGHT = "        Events::Window::Resized::HEIGHT"_hash;
}





#endif  // DR_ENGINE_DATA_TYPES_H

