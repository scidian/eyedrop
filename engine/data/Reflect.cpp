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
#include "engine/scene2d/components/Transform2D.h"

void InitializeReflection() {
    // Create Singleton
    g_reflect = new DrReflect();

    // Register Structs / Classes
    RegisterClass<Transform2D>();
}



