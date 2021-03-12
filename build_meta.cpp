//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
#include "engine/data/Meta.h"

//####################################################################################
//##    Register Components / Properties with Meta Data System
//####################################################################################
//##
#define REGISTER_META_DATA                  // Tell headers we want to register data
//##
//##    !!!!! NOTE: Include all Components below
//##
#include "engine/scene2d/components/Transform.h"




//####################################################################################
//####################################################################################
//####################################################################################
//##    Local Static Variables
//####################################################################################
static std::unordered_map<const char*, ComponentData>   l_components    { };        // Holds data about DrComponent / ECS Component structs and its Properties


//####################################################################################
//##    Meta Data Functions
//####################################################################################
void AddMetaComponent(const char* type_name, std::string comp_name, std::string comp_description) {
    ComponentData comp_data {
        .name =             comp_name,
        .description =      comp_description,
    };
    l_components.insert(std::make_pair(type_name, comp_data));
}

void AddMetaProperty(const char* type_name, std::string prop_name, std::string prop_description) {
    PropertyData (prop_data) {
        .name =             prop_name,
        .description =      prop_description,
    };
    assert(l_components.find(type_name) != l_components.end() && "Component never set with AddMetaComponent before calling AddMetaProperty!");
    l_components[type_name].properties.insert(std::make_pair(l_components[type_name].properties.size(), prop_data));
}



