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
#include <typeinfo>
#include "engine/data/Types.h"

// Forward Declarations
class DrApp;
class DrReflect;

//####################################################################################
//##    Global Variable Declarations
//####################################################################################
extern DrReflect*       g_reflect;                                                  // Meta Data singleton      Defined in Reflect.cpp      Assigned in InitializeReflection()


//####################################################################################
//##    Component / Property data structs
//############################
struct ComponentData {
    ComponentData() { }
    ComponentData(std::string comp_name, std::string comp_title, std::string about, unsigned int show_color, Component_Icon show_icon, bool hide) : 
        name(comp_name), title(comp_title), description(about), color(show_color), icon(show_icon), hidden(hide) { }
    std::string         name            { "Unknown" };                              // Actual struct / class name 
    std::string         title           { "Not Found" };                            // Display name of this Component
    std::string         description     { "Could not find component." };            // Description to show in Help Advisor
    unsigned int        color           { 0xFFFFFFFF };                             // Color of Header in Inspector
    Component_Icon      icon            { Component_Icon::None };                   // enum     Mini icon to show in Inspector
    bool                hidden          { false };                                  // Should this Component appear in Inspector?
};

struct PropertyData {
    PropertyData() { }
    PropertyData(std::string prop_name, std::string prop_title, std::string about, Property_Type prop_type, int off, size_t size_of, bool hide) : 
        name(prop_name), title(prop_title), description(about), type(prop_type), offset(off), size(size_of), hidden(hide) { }
    std::string         name            { "unknown" };                              // Actual member variable name 
    std::string         title           { "Not Found" };                            // Display name of this Property
    std::string         description     { "Could not find property." };             // Description to show in Help Advisor
    Property_Type       type            { Property_Type::Unknown };                 // Type info for how to display in Inspector
    int                 offset          { 0 };                                      // char* offset of member variable within parent component struct
    size_t              size            { 0 };                                      // size of actual type of Property
    bool                hidden          { false };                                  // Should this Property appear in Inspector?
};


//####################################################################################
//##    Type Definitions
//############################
typedef std::map<int, PropertyData> PropertyMap;                                    // Holds a collection of PropertyData, sorted by offset

//####################################################################################
//##    DrReflect
//##        Designed to be a singleton to hold Component / Property 
//##        reflection and meta data
//############################
class DrReflect 
{
public:
    std::unordered_map<const char*, ComponentData>      components      { };        // Holds data about DrComponent / ECS Component structs
    std::unordered_map<const char*, PropertyMap>        properties      { };        // Holds data about Properies (of Components)

public:    
    void AddMetaComponent(const char* type_name, ComponentData comp_data) {
        components.insert(std::make_pair(type_name, comp_data));
    }
    void AddMetaProperty(const char* type_name, PropertyData prop_data) {
        assert(components.find(type_name) != components.end() && "Component never set with AddMetaComponent before calling AddMetaProperty!");
        properties[type_name][prop_data.offset] = prop_data;
    }
};


//####################################################################################
//##    Reflection Declarations
//############################
void                InitializeReflection();                                         // Creates DrReflect class and registers classes and member variables


//####################################################################################
//##    Reflection Data Fetching
//############################
// Meta Data component fetching
template<typename T>
ComponentData GetComponentData() {
    const char* type_name = typeid(T).name();
    if (g_reflect->components.find(type_name) != g_reflect->components.end()) {
        return g_reflect->components[type_name];
    } else {
        return ComponentData();
    }
}

// Meta Data property fetching by Index
template<typename T>
PropertyData GetPropertyData(int property_number) {
    const char* type_name = typeid(T).name();
    int count = 0;
    for (auto prop : g_reflect->properties[type_name]) {
        if (count == property_number) return prop.second;
        ++count;
    }
    return PropertyData();
}

// Get member variable by name using memcpy and offsetof
template<typename T>
void GetProperty(void* component, void* value, int property_number) {
    // Casting from void*, not fully standardized across compilers
    //      vector ex:  std::vector<double> rotation = *(std::vector<double>*)(component_ptr + GetPropertyData<Transform2D>(property_number).offset);
    //      DrVec  ex:  DrVec3 rotation = *(DrVec3*)(p + GetPropertyData<Transform2D>(1).offset);
    // C++ way
    //      DrVec3 rotation;
    //      static constexpr auto offset_rotation = &Transform2D::rotation;
    //      auto r = ((&et)->*off_rot);
    //      rotation = r;
    PropertyData prop_data = GetPropertyData<T>(property_number);
    char* component_ptr = (char*)(component);
    memcpy(value, component_ptr + prop_data.offset, prop_data.size);
}



//####################################################################################
//##    Reflection Registration
//############################
// Template wrapper to register type with DrReflect from header files
template <typename T>
void RegisterClass() {};

// Call this to register class / struct type with reflection / meta data system
template<typename T>
void RegisterComponent(ComponentData comp_data) { 
	g_reflect->AddMetaComponent(typeid(T).name(), comp_data); 
}

// Call this to register member variable with reflection / meta data system
template<typename T>
void RegisterProperty(PropertyData prop_data) {
	g_reflect->AddMetaProperty(typeid(T).name(), prop_data); 
} 


#endif  // DR_META_H

