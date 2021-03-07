//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef ENUMS_TYPES_H
#define ENUMS_TYPES_H

// Global Enum Constants
#define DROP_VERSION        2000                    // Drop Creator version number, important for file loading / saving through Cereal

#define KEY_NONE            -1                      // Value that represents no item selected
#define KEY_SAME            -100                    // Value signifying to use the value already obtained
#define KEY_START           1001                    // Starting value for key generators


//####################################################################################
//##    Used to track what the DrVariant m_value data type really is
//##        Numbers are SAVE FILE CRITICAL and CANNOT BE CHANGED once set
//##
//##    !!!!! NEXT AVAILABLE NUMBER:    33
//##
//############################
enum class Property_Type {
    Bool            =  1,   // bool                 True or False
    BoolDouble      =  2,   // vector<variant>      True or False, when True shows double spinbox,  sizeof 6 - bool: value1, doubles: value2, min, max, step size, string: spinText
    BoolInt         =  3,   // vector<variant>      True or False, when True shows int spinbox,     sizeof 6 - bool: value1, ints:    value2, min, max, step size, string: spinText
    
    Int             =  5,   // long                 Integer
    Positive        =  6,   // long                 Integer >= 0
    RangedInt       =  7,   // vector<variant>      Integer >= min and <= max,                      sizeof 4 - longs: value, min, max, step size

    Double          =  8,   // double               Decimal
    Angle           =  9,   // double               Decimal for showing degrees, shows degree symbol in spinbox
    PositiveDouble  = 10,   // double               Decimal >= 0.0
    RangedDouble    = 11,   // vector<variant>      Decimal >= min and <= max,                      sizeof 4 - doubles: value, min, max, step size
    Slider          = 12,   // vector<variant>      Decimal >= min and <= max, with a slider        sizeof 5 - doubles: value, min, max, step size, string: suffix ("%", etc)
    Percent         = 13,   // double               Decimal from 0.0 to 100.0, shows percent symbol, with a slider

    String          = 14,   // string               Uses QLineEdit
    Textbox         = 15,   // string               Uses QTextEdit for multi-line html text input

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
//##        - All Entities inherit DrSettings to use DrComponents which contain DrProperties compatible with the Inspector
//##
//############################
enum class DrType {    
    // Misc Types
    NotFound = 0,                   // For passing a value in functions that represents No Type Selected / Found, !!!!! #NOTE: Keep as zero

    // Shared Types
    Image,                          // Entities contained within DrProject::m_images

    // Mapped Types
    World,                          // Entities contained within 
    Scene,                          // Entities contained within DrProject::m_scenes
        Entity,                     // Entities contained within DrProject::DrScene::m_entities

};


// ################## Project Mapped Types ####################
// Worlds,    Entities contained within DrProject::m_worlds / DrProject::m_uis
enum class DrWorldType {
    // Editor Worlds
    Physics_2D,
    UI,
};


// Things,    Entities contained within DrProject::DrScenes::m_entities
enum class DrEntityType {
    None,

    // ***** 2D Physics World Sub Types
    Character,
    Object,
};


#endif  // ENUMS_TYPES_H

