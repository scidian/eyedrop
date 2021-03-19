//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
#include <functional>
#include <vector>
#include "engine/data/Reflect.h"


//####################################################################################
//##    Global Variable Definitions
//####################################################################################
DrReflect*                              g_reflect           { nullptr };            // Meta Data singleton      Declared in Reflect.h       Assigned in InitializeReflection()
std::vector< std::function<void()> >    l_fn_list           { };                    // Keeps list of registration functions added by header defines below

//####################################################################################
//####################################################################################
//####################################################################################
//##    Register Components / Properties with Meta Data System
//##            !!!! Include all Components below !!!!
//####################################################################################
#define REGISTER_REFLECTION         // Signal to headers to define register function
#include "engine/scene2d/components/Test1.h"
#include "engine/scene2d/components/Transform2D.h"




//####################################################################################
//####################################################################################
//####################################################################################
//##    General Registration
//####################################################################################
// Initializes global reflection object, registers classes with Reflection System
void InitializeReflection() {
    // Create Singleton
    g_reflect = new DrReflect();                                                    
    
    // Register Structs / Classes
    for (int func = 0; func < l_fn_list.size(); ++func) {
        l_fn_list[func]();       
    }   
}

// Used in registration macros to automatically create nice display name from class / member variable names
void CreateTitle(std::string& name) {
    std::replace(name.begin(), name.end(), '_', ' '); \
    name[0] = toupper(name[0]);
    for (int c = 1; c < name.length(); c++) {
        if (name[c - 1] == ' ') name[c] = toupper(name[c]);
    }
}


//####################################################################################
//##    Class / Member Registration
//####################################################################################
// Update Component data
void RegisterComponent(ComponentData comp_data) { 
	g_reflect->AddMetaComponent(comp_data); 
}

// Update Property data
void RegisterProperty(ComponentData comp_data, PropertyData prop_data) {                     
	g_reflect->AddMetaProperty(comp_data, prop_data); 
} 


//####################################################################################
//##    Data Fetching
//####################################################################################
// Meta Data component fetching from passed in component typeid().hash_code()
ComponentData GetComponentData(HashID hash_id) {
    for (auto& pair : g_reflect->components) {
        if (pair.first == hash_id) return pair.second;
    }
    return ComponentData();
}

// Meta Data property fetching by member variable Index and component typeid().hash_code()
PropertyData GetPropertyData(HashID component_hash_id, int property_number) {
    int count = 0;
    for (auto& prop : g_reflect->properties[component_hash_id]) {
        if (count == property_number) return prop.second;
        ++count;
    }
    return PropertyData();    
}

// Meta Data property fetching by member variable Name and component typeid().hash_code()
PropertyData GetPropertyData(HashID component_hash_id, std::string property_name) {
    for (auto& prop : g_reflect->properties[component_hash_id]) {
        if (prop.second.name == property_name) return prop.second;
    }
    return PropertyData();
}

