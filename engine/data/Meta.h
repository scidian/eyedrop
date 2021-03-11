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
#include <string>


//####################################################################################
//##    Component / Property data structs
//############################
struct ComponentData {
    std::string     name;
    std::string     description;
};

struct PropertyData {
    std::string     name;
    std::string     description;
};


//####################################################################################
//##    Meta Data Funciton Declarations
//############################
void AddMetaComponent(const char* type_name, std::string comp_name, std::string description);


//####################################################################################
//##    Template Class, calls this funciton that registers the meta data at initialization
//############################
template<typename T>
class RegisterComponentData { 
public:
    RegisterComponentData(std::string comp_name, std::string description) { 
        const char* type_name = typeid(T).name();
	    AddMetaComponent(type_name, comp_name, description); 
    } 
};

#endif  // DR_META_H

