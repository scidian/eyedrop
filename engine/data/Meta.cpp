//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 11 2021
//
//
#include "engine/data/Meta.h"


//####################################################################################
//##    Register Components / Properties with Meta Data System
//##            !!!! Include all Components below !!!!
//####################################################################################
#define REGISTER_META_DATA            // Signal to headers to define register function
#include "engine/scene2d/components/Transform2D.h"

void DrMeta::InitializeMetaData() {
    RegisterMetaData<Transform2D>();
}


//####################################################################################
//##    DrMeta Functions
//####################################################################################
void DrMeta::AddMetaComponent(const char* type_name, std::string comp_name, std::string comp_description) {
    ComponentData comp_data { };
        comp_data.name =            comp_name;
        comp_data.description =     comp_description;
    components.insert(std::make_pair(type_name, comp_data));
}

void DrMeta::AddMetaProperty(const char* type_name, std::string prop_name, std::string prop_description, int offset) {
    PropertyData prop_data { };
        prop_data.name =            prop_name;
        prop_data.description =     prop_description;
        prop_data.offset =          offset;
        prop_data.type =            Property_Type::Unknown;
    assert(components.find(type_name) != components.end() && "Component never set with AddMetaComponent before calling AddMetaProperty!");
    properties[type_name][offset] = prop_data;
}



