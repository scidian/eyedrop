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
#include "../data/Declares.h"
#include "Data.h"
#include "Variant.h"

struct DrProperty {

};

// Type Definitions
typedef std::unordered_map<std::string, DrProperty>    Properties;


//####################################################################################
//##    DrComponent
//##        Interface class to hold an Entity Component
//############################
class DrComponent 
{
    // Constructor
    DrComponent(long entity_id_, std::string component_name) : entity_id(entity_id_) { }
    virtual ~DrComponent() { }


    // #################### VARIABLES ####################
public:
    // Local Variables
    std::string                 name;                                               // Name of this Component
    long                        entity_id               { KEY_NONE };               // ID Key of parent Entity
    long                        id                      { KEY_NONE };               // ID Key of this Component
    Properties                  properties;                                         // Holds extended attributes for this Component by Property name (as std::string)


    // #################### PROPERTY REFLECTION ####################
public:
    virtual void initializeProperties() {
        //properies.addProperty();
    }

    virtual void propertyDescription(const std::string& name) {

    }


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    // Component Events
    virtual void        init() = 0;                                                                 // Called when component is first created in active Scene
    virtual void        addedToScene() = 0;                                                         // Called when Entity is added to an active Scene
    virtual void        draw() = 0;                                                                 // Called when it is time to Render Entity
    virtual bool        update(double time_passed, double time_warp) = 0;                           // Called during Scene->update() step
    virtual void        destroy() = 0;                                                              // Called when component is being removed from an active Scene


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Getters / Setters
    

};

#endif  // DR_COMPONENT_H

