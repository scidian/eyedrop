//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Tue Mar 09 2021
//
//
#ifndef DR_REFLECTION_H
#define DR_REFLECTION_H

#include "core/imaging/Color.h"
#include "../data/Declares.h"


//####################################################################################
//##    ComponentInfo
//##        Information to know about each Component
//############################
struct ComponentInfo {
    std::string         display_name            { "Unknown Component" };            // Name to be displayed in Editor
    std::string         description             { "No description." };              // Description to be displayed in Editor
    std::string         icon                    { "" };                             // Icon string
    DrColor             color                   { DrColor(128, 128, 128, 255) };    // Object Inspector / Node Box header color
    bool                is_hidden               { true };                           // Should this Component be hidden from Object Insepctor?

};

//####################################################################################
//##    PropertyInfo
//##        Information to know about each Property
//############################
struct PropertyInfo {
    Property_Type       property_type           { Property_Type::String };          // What this Property represents
    std::string         display_name            { "Unknown Property" };             // Name to be displayed in Editor
    std::string         description             { "No description." };              // Description to be displayed in Editor
    bool                is_hidden               { false };                          // Should this appear in the Inspector
    bool                is_editable             { true };                           // Should this be editable in the Inspector
};


//####################################################################################
//##    DrMeta
//##        Holds meta information about Components and Properties used throughout Drop
//############################
class DrMeta 
{
public:
    // Constructor / Destructor
    DrMeta(std::shared_ptr<DrEntity> parent_entity = nullptr, std::string entity_name = "") { }
    ~DrMeta() { }

    // #################### VARIABLES ####################
public:
    // External Borrowed Pointers
    std::shared_ptr<DrMeta>     parent                  { nullptr };                // Pointer to the parent Entity
    long                        parent_id               { KEY_NONE };               // ID Key of parent Entity

    // Local Variables
    std::string                 name                    { "" };                     // Name of this Entity
    long                        id                      { KEY_NONE };               // ID Key of this Entity

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    //int                 getVariable() { return m_variable; }
    //void                setVariable(int variable) { m_variable = variable; }

};



#endif  // DR_REFLECTION_H