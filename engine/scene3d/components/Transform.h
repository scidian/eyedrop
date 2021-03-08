//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 08 2021
//
//
#ifndef DR_COMPONENT_TRANSFORM_H
#define DR_COMPONENT_TRANSFORM_H

// Includes
#include "engine/entity/Component.h"


//####################################################################################
//##    CompTransform
//##        Interface class to hold an Entity Component
//############################
class CompTransform : public DrComponent 
{
    // Constructor
    CompTransform(std::shared_ptr<DrEntity> owner_entity, std::string component_name) : DrComponent(owner_entity, component_name) { };
    virtual ~CompTransform() { };


    // #################### VARIABLES ####################
public:


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    // Component Events
    virtual void        init() { };                                                                 // Called when component is first created in active Scene
    virtual void        addedToScene() { };                                                         // Called when Entity is added to an active Scene
    virtual void        draw() { };                                                                 // Called when it is time to Render Entity
    virtual bool        update(double time_passed, double time_warp) { };                           // Called during Scene->update() step
    virtual void        destroy() { };                                                              // Called when component is being removed from an active Scene


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Getters / Setters
    

};

#endif  // DR_COMPONENT_TRANSFORM_H

