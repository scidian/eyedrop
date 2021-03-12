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
#include <string>


//####################################################################################
//##    Component / Property data structs
//############################
struct ComponentData {
    std::string     name;
    std::string     description;
    // ##### Internal
    int             property_count = 0;         // DO NOT SET! Automated increase during call to AddMetaProperty()
};

struct PropertyData {
    std::string     name;
    std::string     description;
};


//####################################################################################
//##    Meta Data Funciton Declarations
//############################
void AddMetaComponent   (const char* type_name, std::string comp_name, std::string description);
void AddMetaProperty    (const char* type_name, std::string prop_name, std::string description);


//####################################################################################
//##    Template Classes
//##        Define these classes to register Meta Data
//############################
template<typename T>
class RegisterMetaComponent { 
public:
    RegisterMetaComponent(std::string comp_name, std::string description) { 
        const char* type_name = typeid(T).name();
	    AddMetaComponent(type_name, comp_name, description); 
    } 
};

template<typename T>
class RegisterMetaProperty { 
public:
    RegisterMetaProperty(std::string prop_name, std::string description) { 
        const char* type_name = typeid(T).name();
	    AddMetaProperty(type_name, prop_name, description); 
    } 
};

#endif  // DR_META_H

