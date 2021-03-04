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
constexpr int       c_no_key =                  -1;                 // Value that represents no item selected
constexpr int       c_same_key =                -100;               // Value signifying to use the value already obtained

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


// Things,    Entities contained within DrProject::DrScenes::m_things
enum class DrEntityType {
    None,

    // ***** 2D Physics World Sub Types
    Character,
    Object,
};


#endif  // ENUMS_TYPES_H

