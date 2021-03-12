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
struct PropertyData {
    std::string                     name            { "" };
    std::string                     description     { "" };    
};

struct ComponentData {
    std::string                     name            { "" };
    std::string                     description     { "" };
    std::map<int, PropertyData>     properties      { };
};


//####################################################################################
//##    Meta Data Funciton Declarations
//############################
void AddMetaComponent   (const char* type_name, std::string comp_name, std::string comp_description);
void AddMetaProperty    (const char* type_name, std::string prop_name, std::string prop_description);


//####################################################################################
//##    Template Classes
//##        Define these classes to register Meta Data
//############################
template<typename T>
class RegisterMetaComponent { 
public:
    RegisterMetaComponent(std::string comp_name, std::string comp_description) { 
        const char* type_name = typeid(T).name();
	    AddMetaComponent(type_name, comp_name, comp_description); 
    } 
};

template<typename T>
class RegisterMetaProperty { 
public:
    RegisterMetaProperty(std::string prop_name, std::string prop_description) { 
        const char* type_name = typeid(T).name();
	    AddMetaProperty(type_name, prop_name, prop_description); 
    } 
};

#endif  // DR_META_H

