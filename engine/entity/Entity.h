//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_ENTITY_H
#define DR_ENTITY_H

// Includes
#include "../data/Declares.h"
#include "Component.h"


//####################################################################################
//##    DrEntity
//##        Interface class to hold a Scene Object
//############################
class DrEntity 
{
public:
    // Constructor / Destructor
    DrEntity(std::string name_ = "", long parent_id_ = KEY_NONE);
    ~DrEntity();

    // #################### VARIABLES ####################
public:
    // Local Variables
    std::string                 name                    { "" };                     // Name of this Entity
    long                        id                      { KEY_NONE };               // ID Key of this Entity
    Components                  components              { };                        // Components of this Entity       

    // Relationship Variables
    long                        project_id              { KEY_NONE };               // FOR GAME ENTITIES ONLY!! Key to original starting data Project::Entity    
    long                        parent_id               { KEY_NONE };               // ID Key of parent Entity, if applicable
    Entities                    children                { };                        // Children of this Entity
    
    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    //int                 getVariable() { return m_variable; }
    //void                setVariable(int variable) { m_variable = variable; }

};


#endif  // DR_ENTITY_H