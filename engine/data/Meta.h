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
#include <map>
#include <unordered_map>
#include "engine/data/Types.h"

// Forward Declarations
class DrApp;
class DrMeta;

//####################################################################################
//##    Global Declarations
//####################################################################################
extern DrApp*               g_app;                                                  // App singleton            Defined in App.cpp      Assigned in App.cpp::App()
extern DrMeta*              g_meta;                                                 // Meta Data singleton      Defined in App.cpp      Assigned in Editor.cpp::main()


//####################################################################################
//##    Component / Property data structs
//############################
struct ComponentData {
    std::string         name            { "unknown" };                              // Actual struct / class name 
    std::string         display         { "Not Found" };                            // Display name of this Component
    std::string         description     { "Could not find component." };            // Description to show in Help Advisor
    unsigned int        color           { 0xFFFFFFFF };                             // Color of Header in Inspector
    Component_Icon      icon            { Component_Icon::None };                   // enum     Mini icon to show in Inspector
    bool                hidden          { false };                                  // Should this Component appear in Inspector?
};

struct PropertyData {
    std::string         name            { "unknown" };                              // Actual member variable name 
    std::string         display         { "Not Found" };                            // Display name of this Property
    std::string         description     { "Could not find property." };             // Description to show in Help Advisor
    int                 offset          { 0 };                                      // char* offset of member variable within parent component struct
    Property_Type       type            { Property_Type::Unknown };                 // Type info for how to display in Inspector
    bool                hidden          { false };                                  // Should this Property appear in Inspector?
};


//####################################################################################
//##    Type Definitions
//############################
typedef std::map<int, PropertyData> PropertyMap;                                    // Holds a collection of PropertyData, sorted by offset

//####################################################################################
//##    DrMeta
//##        Designed to be a singleton to hold Component / Property meta data
//############################
class DrMeta 
{
public:
    std::unordered_map<const char*, ComponentData>      components      { };        // Holds data about DrComponent / ECS Component structs
    std::unordered_map<const char*, PropertyMap>        properties      { };        // Holds data about Properies (of Components)

public:
    void            InitializeMetaData();
    void            AddMetaComponent(const char* type_name, std::string comp_name, std::string comp_description);
    void            AddMetaProperty (const char* type_name, std::string prop_name, std::string prop_description, int offset);

};


//####################################################################################
//##    Template Classes
//############################
// Meta Data component fetching
template<typename T>
ComponentData GetMetaComponent() {
    const char* type_name = typeid(T).name();
    //assert(l_components.find(type_name) != l_components.end() && "Could not find component in meta data!");
    return g_meta->components[type_name];
}

// Meta Data property fetching by Index
template<typename T>
PropertyData GetMetaProperty(int property_number) {
    const char* type_name = typeid(T).name();
    int count = 0;
    for (auto prop : g_meta->properties[type_name]) {
        if (count == property_number) return prop.second;
        ++count;
    }
    return PropertyData();
}

// Wrapper to Register Meta Data from header files
template <typename T>
void RegisterMetaData() {};

// Define this class to register Meta Data
template<typename T>
void RegisterMetaComponent(std::string comp_name, std::string comp_description) { 
    const char* type_name = typeid(T).name();
	g_meta->AddMetaComponent(type_name, comp_name, comp_description); 
}

// Define this classes to register Meta Data
template<typename T>
void RegisterMetaProperty(std::string prop_name, std::string prop_description, int offset) { 
    const char* type_name = typeid(T).name();
	g_meta->AddMetaProperty(type_name, prop_name, prop_description, offset); 
} 

#endif  // DR_META_H

