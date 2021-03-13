//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
#ifndef DR_META_H
#define DR_META_H

// Includes
#include "engine/data/Types.h"


//####################################################################################
//##    Component / Property data structs
//############################
struct ComponentData {
    std::string     name            { "" };
    std::string     description     { "" };
};

struct PropertyData {
    std::string     name            { "" };
    std::string     description     { "" };    
    Property_Type   type            { Property_Type::Unknown };
};
typedef std::map<int, PropertyData>         PropertyMap;


//####################################################################################
//##    Meta Data Funciton Declarations
//############################
void            RegisterMetaData();

void            AddMetaComponent   (const char* type_name, std::string comp_name, std::string comp_description);
void            AddMetaProperty    (const char* type_name, std::string prop_name, std::string prop_description);

ComponentData   FetchMetaComponent (const char* type_name);
PropertyData    FetchMetaProperty  (const char* type_name, int property_number);


//####################################################################################
//##    Template Classes
//############################
// Meta Data component fetching
template<typename T>
ComponentData GetMetaComponent() {
    const char* type_name = typeid(T).name();
    return FetchMetaComponent(type_name);
}

// Meta Data property fetching
template<typename T>
PropertyData GetMetaProperty(int property_number) {
    const char* type_name = typeid(T).name();
    return FetchMetaProperty(type_name, property_number);
}

// Define this class to register Meta Data
template<typename T>
void RegisterMetaComponent(std::string comp_name, std::string comp_description) { 
    const char* type_name = typeid(T).name();
	AddMetaComponent(type_name, comp_name, comp_description); 
}

// Define this classes to register Meta Data
template<typename T>
void RegisterMetaProperty(std::string prop_name, std::string prop_description) { 
    const char* type_name = typeid(T).name();
	AddMetaProperty(type_name, prop_name, prop_description); 
} 

#endif  // DR_META_H

