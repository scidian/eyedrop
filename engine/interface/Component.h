//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_COMPONENT_H
#define DR_COMPONENT_H

// Includes
#include <map>
#include <string>
#include "core/Variant.h"

// Type Definitions
typedef std::map<std::string, DrVariant>    Properties;


//####################################################################################
//##    DrComponent
//##        Interface class to hold an Entity Component
//############################
class DrComponent 
{
    // Constructor
    DrComponent(DrScene *owner_scene, DrEntity *owner_entity, std::string component_name);
    virtual ~DrComponent();


    // #################### VARIABLES ####################
public:
    // External Borrowed Pointers
    DrScene            *scene         { nullptr };                          // Pointer to the parent Scene
    DrEntity           *entity        { nullptr };                          // Pointer to the parent Entity

    // Local Variables
    std::string         name;                                               // Name of this Component
    Properties          m_properties;                                       // Holds variables for this Component by Property name (as std::string)


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    // Component Events
    virtual void        init() = 0;                                                                 // Called when component is first created in active Scene
    virtual void        addedToScene() = 0;                                                         // Called when Entity is added to active Scene
    virtual void        draw() = 0;                                                                 // Called when it is time to Render Entity
    virtual bool        update(double time_passed, double time_warp) = 0;                           // Called during App->onUpdate() step
    virtual void        destroy() = 0;                                                              // Called when component is being removed from active Scene


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Getters / Setters
    


};


#endif  // DR_COMPONENT_H