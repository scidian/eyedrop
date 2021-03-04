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
private:
    // External Borrowed Pointers
    DrScene            *m_scene         { nullptr };                        // Pointer to the parent Scene
    DrEntity           *m_entity        { nullptr };                        // Pointer to the parent Entity

    // Local Variables
    std::string         m_name;                                             // Name of this component
    Properties          m_properties;                                       // Holds variables for this component by name (as std::string)


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    // Component Events
    virtual void        init();                                                                     // Called when component is first created in active Scene
    virtual void        addedToScene();                                                             // Called when Entity is added to active Scene
    virtual void        draw();                                                                     // Called when it is time to Render Entity
    virtual bool        update(double time_passed, double time_warp);                               // Called during App->onUpdate() step
    virtual void        destroy();                                                                  // Called when component is being removed from active Scene

    // Getters
    std::string         name()      { return m_name; }                                              // Returns name of this Component
    DrScene*            scene()     { return m_world; }                                             // Returns parent Scene
    DrEntity*           entity()    { return m_entity; }                                            // Returns parent Entity
    
    // Attributes
    DrVariant           attribute(std::string attribute_name);                                      // Returns attribute by name
    void                setAttribute(std::string attribute_name, DrVariant value);                  // Sets attribute by name


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Getters / Setters
    


};


#endif  // DR_COMPONENT_H