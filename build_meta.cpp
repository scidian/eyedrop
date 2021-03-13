//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
#include <map>
#include <unordered_map>
#include "engine/data/Meta.h"

//####################################################################################
//##    Register Components / Properties with Meta Data System
//##            !!!! Include all Components below !!!!
//####################################################################################
#define REGISTER_META_DATA            // Signal to headers to define register function
#include "engine/scene2d/components/Transform2D.h"

void RegisterMetaData() {
    RegisterComponentTransform2D();
}



//####################################################################################
//####################################################################################
//####################################################################################
//##    Local Static Variables
//####################################################################################
typedef std::map<int, PropertyData> PropertyMap;
static std::unordered_map<const char*, ComponentData>   l_components    { };        // Holds data about DrComponent / ECS Component structs
static std::unordered_map<const char*, PropertyMap>     l_properties    { };        // Holds data about Properies (of Components)


//####################################################################################
//##    Meta Data Functions
//####################################################################################
ComponentData FetchMetaComponent(const char* type_name) {
    //assert(l_components.find(type_name) != l_components.end() && "Could not find component in meta data!");
    return l_components[type_name];
}

PropertyData FetchMetaProperty(const char* type_name, int property_number) {
    return l_properties[type_name][property_number];
}

void AddMetaComponent(const char* type_name, std::string comp_name, std::string comp_description) {
    ComponentData comp_data { };
        comp_data.name =             comp_name;
        comp_data.description =      comp_description;
    l_components.insert(std::make_pair(type_name, comp_data));
}

void AddMetaProperty(const char* type_name, std::string prop_name, std::string prop_description) {
    PropertyData prop_data { };
        prop_data.name =             prop_name;
        prop_data.description =      prop_description;
    assert(l_components.find(type_name) != l_components.end() && "Component never set with AddMetaComponent before calling AddMetaProperty!");
    l_properties[type_name][l_properties[type_name].size()] = prop_data;
}



