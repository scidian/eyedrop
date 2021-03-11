//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
//####################################################################################
//##
//##    Register Meta Data for Components / Properties
//##
//####################################################################################

// Includes
#include <string>
#include <unordered_map>
#include "engine/data/Meta.h"

//####################################################################################
//##    Local Static Variables
//####################################################################################
static std::unordered_map<const char*, ComponentData>   l_components    { };        // Holds data about DrComponents / ECS Components
static std::unordered_map<const char*, PropertyData>    l_properties    { };        // Holds data about DrProperies / ECS Properties


//####################################################################################
//##    Register Components
//####################################################################################
#define REGISTER_META_DATA
#include "engine/scene2d/components/Transform.h"


//####################################################################################
//##    Meta Data Functions
//####################################################################################
void AddMetaComponent(const char* type_name, std::string comp_name, std::string description) {
    ComponentData (data) {
        .name = comp_name,
        .description = description,
    };
    l_components.insert({type_name, data});
}



