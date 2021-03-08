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
#include <map>
#include <string>
#include <unordered_map>
#include "engine/data/Types.h"

// Forward Declarations
class DrComponent;

// Type Definitions
typedef std::unordered_map<long, std::shared_ptr<DrComponent>>    Components;
typedef std::unordered_map<long, std::shared_ptr<DrEntity>>       Entities;


//####################################################################################
//##    DrEntity
//##        Interface class to hold a Scene Object
//############################
class DrEntity 
{
public:
    // Constructor / Destructor
    DrEntity(std::shared_ptr<DrEntity> owner_entity = nullptr, std::string entity_name = "") { };
    ~DrEntity() { };

    // #################### VARIABLES ####################
public:
    // Local Variables
    std::string                 name                    { "" };                     // Name of this Entity
    long                        id                      { KEY_NONE };               // ID Key of this Entity
    Entities                    children                { };                        // Children of this Entity
    Components                  components              { };                        // Components of this Entity       

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    //int                 getVariable() { return m_variable; }
    //void                setVariable(int variable) { m_variable = variable; }

};


#endif  // DR_ENTITY_H