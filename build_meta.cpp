//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
//####################################################################################
//##    Register Components / Properties
//####################################################################################
#include "engine/data/Meta.h"

// Tell headers we want to register data
#define REGISTER_META_DATA

//####################################################################################
//####################################################################################
//####################################################################################
//############################ Include all Components Here!!!!!
#include "engine/scene2d/components/Transform.h"





//####################################################################################
//####################################################################################
//####################################################################################

//####################################################################################
//##    Local Static Variables
//####################################################################################
static std::map<const char*, ComponentData>                     l_components    { };        // Holds data about DrComponent / ECS Component structs
static std::map<std::pair<const char*, int>, PropertyData>      l_properties    { };        // Holds data about Properies (of Components)

//####################################################################################
//##    Meta Data Functions
//####################################################################################
void AddMetaComponent(const char* type_name, std::string comp_name, std::string description) {
    ComponentData (comp_data) {
        .name =             comp_name,
        .description =      description,
        .property_count =   0,
    };
    l_components.insert({type_name, comp_data});
}

void AddMetaProperty(const char* type_name, std::string prop_name, std::string description) {
    PropertyData (prop_data) {
        .name =             prop_name,
        .description =      description,
    };
    assert(l_components.find(type_name) != l_components.end() && "Component never set with AddMetaComponent before calling AddMetaProperty!");
    int property_number = l_components[type_name].property_count++;
    l_properties.insert({std::make_pair(type_name, property_number), prop_data});
}



