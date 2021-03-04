//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_SCENE_H
#define DR_SCENE_H

// Includes
#include <map>

// Forward Declarations
class DrEntity;


//####################################################################################
//##    DrScene
//##        Interface class to hold a Scene
//############################
class DrScene 
{
public:
    // Constructor / Destructor
    DrScene();
    ~DrScene();

    // #################### VARIABLES ####################
public:
    // Local Variables
    std::string                     name            { };                    // Name of this Scene
    std::map<long, DrEntity*>       children        { };                    // Entities within this Scene

        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    

};


#endif  // DR_ENTITY_H