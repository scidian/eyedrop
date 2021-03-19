//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
#include "engine/data/Reflect.h"

//####################################################################################
//##    Global Variable Definitions
//####################################################################################
DrReflect*          g_reflect   { nullptr };                                        // Meta Data singleton      Declared in Reflect.h       Assigned in InitializeReflection()


//####################################################################################
//##    Register Components / Properties with Meta Data System
//##            !!!! Include all Components below !!!!
//####################################################################################
#define REGISTER_REFLECTION         // Signal to headers to define register function
#include "engine/scene2d/components/Test1.h"
#include "engine/scene2d/components/Transform2D.h"

void InitializeReflection() {
    // Create Singleton
    g_reflect = new DrReflect();

    // Register Structs / Classes
    RegisterClass<Test1>();
    RegisterClass<Transform2D>();
}


//####################################################################################
//####################################################################################
//####################################################################################

//####################################################################################
//##    Non-template Reflection Functions
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

