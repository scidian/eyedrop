//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#ifndef DR_SCENE_H
#define DR_SCENE_H

// Forward Declarations
class DrCoordinator;


//####################################################################################
//##    IScene
//##        Interface for scene containing entities within an entity component system
//############################
class IScene
{
public:
    // Constructor / Destructor
    IScene();
    ~IScene();

    // #################### VARIABLES ####################
private:
    // Local Variables
    DrCoordinator*          m_ecs                   { nullptr };                    // Entity Component System
            

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    DrCoordinator*          ecs()                   { return m_ecs; }

};

#endif  // DR_SCENE_2D_H