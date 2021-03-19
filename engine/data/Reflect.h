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
    std::string         name            { "unknown" };                              // Actual struct / class name 
    HashID              hash_code       { 0 };                                      // typeid().hash_code of actual underlying type of Component
    // ----- Following Meta Data Can Be User Set -----
    std::string         title           { "unknown" };                              // Display name of this Component
    std::string         description     { "No component description." };            // Description to show in Help Advisor
    bool                hidden          { false };                                  // Should this Component appear in Inspector?
    unsigned int        color           { 0xFFFFFFFF };                             // Color of Header in Inspector
    unsigned int        icon            { Component_Icon::None };                   // enum Component_Icon, mini icon to show in Inspector
};

struct PropertyData {
    std::string         name            { "unknown" };                              // Actual member variable name 
    HashID              hash_code       { 0 };                                      // typeid().hash_code of actual underlying type of member variable
    int                 offset          { 0 };                                      // char* offset of member variable within parent component struct
    size_t              size            { 0 };                                      // size of actual type of Property
    // ----- Following Meta Data Can Be User Set -----
    std::string         title           { "unknown" };                              // Display name of this Property
    std::string         description     { "No property description." };             // Description to show in Help Advisor
    bool                hidden          { false };                                  // Should this Property appear in Inspector?
    Property_Type       type            { Property_Type::Unknown };                 // Type info for how to display in Inspector (and what type to use to retrieve value)
};


//####################################################################################
//##    Type Definitions
//############################
typedef std::map<int, PropertyData> PropertyMap;                                    // Holds a collection of PropertyData, sorted by offsetof() value


//####################################################################################
//##    DrReflect
//##        Designed to be a singleton to hold Component / Property 
//##        reflection and meta data
//############################
class DrReflect 
{
public:
    std::unordered_map<HashID, ComponentData>      components      { };             // Holds data about DrComponent / ECS Component structs
    std::unordered_map<HashID, PropertyMap>        properties      { };             // Holds data about Properies (of Components)

public:    
    void AddMetaComponent(ComponentData comp_data) {
        assert(comp_data.hash_code != 0 && "Component hash code is 0, error in registration?");
        components[comp_data.hash_code] = comp_data;
    }
    void AddMetaProperty(ComponentData comp_data, PropertyData prop_data) {
        assert(comp_data.hash_code != 0 && "Component hash code is 0, error in registration?");
        assert(components.find(comp_data.hash_code) != components.end() && "Component never set with AddMetaComponent before calling AddMetaProperty!");
        properties[comp_data.hash_code][prop_data.offset] = prop_data;
    }
};


//####################################################################################
//##    General Functions
//############################
void            InitializeReflection();                                             // Creates DrReflect class and registers classes and member variables
void            CreateTitle(std::string& name);                                     // Create nice display name from class / member variable names
void            RegisterComponent(ComponentData comp_data);                         // Update Component data
void            RegisterProperty(ComponentData comp_data, PropertyData prop_data);  // Update Property data                
	

//####################################################################################
//##    Class / Member Registration
//####################################################################################
// Template wrapper to register type with DrReflect from header files
template <typename T> void RegisterClass() { };                                                

// Call this to register class / struct type with reflection / meta data system, typename CT is Component Type
template <typename CT>
void RegisterComponent(ComponentData comp_data) { 
    assert(std::is_standard_layout<CT>() && "Component is not standard layout!!");
	g_reflect->AddMetaComponent(comp_data); 
}

// Call this to register member variable with reflection / meta data system, typename PT is Property Type
template <typename PT>
void RegisterProperty(ComponentData comp_data, PropertyData prop_data) {        
    assert(typeid(PT).hash_code() != typeid(std::string).hash_code() && "Std::String not supported for property type!!");             
	g_reflect->AddMetaProperty(comp_data, prop_data); 
} 



//####################################################################################
//##    Reflection Data Fetching
//############################
// #################### Component Data Fetching ####################
// Meta Data component fetching by component Class Name
template<typename T>
ComponentData GetComponentData() {
    HashID hash = typeid(T).hash_code();
    if (g_reflect->components.find(hash) != g_reflect->components.end()) {
        return g_reflect->components[hash];
    } else { 
        return ComponentData(); 
    }
}
// Meta Data component fetching from passed in component Instance
template<typename T>
ComponentData GetComponentData(T& component) {
    return GetComponentData<T>();
}
// Meta Data component fetching from passed in component typeid().hash_code()
ComponentData GetComponentData(HashID hash_id);


// #################### Property Data Fetching ####################
// -------------------------    By Index  -------------------------
// Meta Data property fetching by member variable Index and component typeid().hash_code()
PropertyData GetPropertyData(HashID component_hash_id, int property_number);
// Meta Data property fetching by member variable Index and component Class Name
template<typename T>
PropertyData GetPropertyData(int property_number) {
    HashID component_hash_id = typeid(T).hash_code();
    return GetPropertyData(component_hash_id, property_number);   
}
// Meta Data property fetching by member variable Index and component Instance
template<typename T>
PropertyData GetPropertyData(T& component, int property_number) {
    return GetPropertyData<T>(property_number);   
}

// -------------------------    By Name  -------------------------
// Meta Data property fetching by member variable Name and component typeid().hash_code()
PropertyData GetPropertyData(HashID component_hash_id, std::string property_name);
// Meta Data property fetching by member variable Name and component Class Name
template<typename T>
PropertyData GetPropertyData(std::string property_name) {
    HashID component_hash_id = typeid(T).hash_code();
    return GetPropertyData(component_hash_id, property_name); 
}
// Meta Data property fetching by member variable Name and component Instance
template<typename T>
PropertyData GetPropertyData(T& component, std::string property_name) {
    return GetPropertyData<T>(property_name); 
}


// #################### Property Value Fetching ####################
// Get member variable value by Index, using memcpy and offsetof
template<typename ReturnType, typename ComponentType>
ReturnType GetProperty(ComponentType& component, int property_number) {
    // Casting from void*, not fully standardized across compilers?
    //      DrVec3 rotation = *(DrVec3*)(component_ptr + prop_data.offset);
    // Memcpy
    //      DrVec3 value;
    //      memcpy(&value, component_ptr + prop_data.offset, prop_data.size);
    // C++ way
    //      static constexpr auto offset_rotation = &Transform2D::rotation;
    //      DrVec3 rotation = ((&et)->*off_rot);
    PropertyData prop_data = GetPropertyData<ComponentType>(property_number);
    assert(prop_data.name != "unknown" && "Could not find property by index!");
    assert(prop_data.hash_code == typeid(ReturnType).hash_code() && "Did not request proper return type!");
    char* component_ptr = (char*)(&component);
    return *(reinterpret_cast<ReturnType*>(component_ptr + prop_data.offset));
}

// Get member variable value by Name, using memcpy and offsetof
template<typename ReturnType, typename ComponentType>
ReturnType GetProperty(ComponentType& component, std::string property_name) {
    PropertyData prop_data = GetPropertyData<ComponentType>(property_name);
    assert(prop_data.name != "unknown" && "Could not find property by name!");
    assert(prop_data.hash_code == typeid(ReturnType).hash_code() && "Did not request proper return type!");
    char* component_ptr = (char*)(&component);
    return *(reinterpret_cast<ReturnType*>(component_ptr + prop_data.offset));
}

// Get member variable value by Name, using memcpy and offsetof
template<typename ReturnType>
ReturnType GetProperty(void* component, HashID component_hash_id, std::string property_name) {
    PropertyData prop_data = GetPropertyData(component_hash_id, property_name);
    assert(prop_data.name != "unknown" && "Could not find property by name!");
    assert(prop_data.hash_code == typeid(ReturnType).hash_code() && "Did not request proper return type!");
    char* component_ptr = (char*)(component);
    return *(reinterpret_cast<ReturnType*>(component_ptr + prop_data.offset));
}

// SetProperty
// void set_int(void *block, size_t offset, int val) {
//     char *p = block;
//     memcpy(p + offset, &val, sizeof val);
// }


//####################################################################################
//##    Macros for Reflection Registration
//####################################################################################
// Static variable added to class allows Registration Function to be added to list of Components to be registered
#define REFLECT() \
    static bool reflection; \
    static bool initReflection();

// Define Registration Function
#define REFLECT_STRUCT(TYPE) \
    template <> void RegisterClass<TYPE>() { \
        using T = TYPE; \
        ComponentData comp {}; \
			comp.name = #TYPE; \
			comp.hash_code = typeid(TYPE).hash_code(); \
			comp.title = #TYPE; \
            CreateTitle(comp.title); \
		RegisterComponent<T>(comp); \
		int property_number = 0; \
		std::unordered_map<int, PropertyData> props { };

// Meta data functions
#define STRUCT_META_TITLE(STRING) 		comp.title = 		#STRING;	RegisterComponent(comp);
#define STRUCT_META_DESCRIPTION(STRING) comp.description = 	#STRING; 	RegisterComponent(comp);
#define STRUCT_META_HIDDEN(BOOL)     	comp.hidden = 		BOOL; 		RegisterComponent(comp);
#define STRUCT_META_COLOR(UINT) 		comp.color = 		UINT; 		RegisterComponent(comp);
#define STRUCT_META_ICON(INT) 			comp.icon = 		INT; 		RegisterComponent(comp);

// Property Registration
#define REFLECT_MEMBER(MEMBER) \
		property_number++; \
		props[property_number] = PropertyData(); \
		props[property_number].name = #MEMBER; \
		props[property_number].hash_code = typeid(T::MEMBER).hash_code(); \
		props[property_number].offset = offsetof(T, MEMBER); \
		props[property_number].size = sizeof(T::MEMBER); \
		props[property_number].title = #MEMBER; \
        CreateTitle(props[property_number].title); \
		RegisterProperty<decltype(T::MEMBER)>(comp, props[property_number]); 

// Meta data functions
#define MEMBER_META_TITLE(STRING) 		props[property_number].title = 			#STRING;	RegisterProperty(comp, props[property_number]); 
#define MEMBER_META_DESCRIPTION(STRING) props[property_number].description = 	#STRING; 	RegisterProperty(comp, props[property_number]); 
#define MEMBER_META_HIDDEN(BOOL)     	props[property_number].hidden = 		BOOL; 		RegisterProperty(comp, props[property_number]); 
#define MEMBER_META_TYPE(INT) 			props[property_number].type = 			INT; 		RegisterProperty(comp, props[property_number]); 

// Static definitions add Registration Function to list of Components to be registered
#define REFLECT_END(TYPE) \
    } \
    bool TYPE::reflection { initReflection() }; \
    bool TYPE::initReflection() { \
        l_fn_list.push_back(std::bind(&RegisterClass<TYPE>)); \
        return true; \
    }

#endif  // DR_META_H

