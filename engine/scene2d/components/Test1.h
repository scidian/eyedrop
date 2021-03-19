//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 18 2021
//
//
#ifndef DR_COMP_TEST1_H
#define DR_COMP_TEST1_H

#include "core/imaging/Color.h"
#include "engine/data/Reflect.h"


//####################################################################################
//##    ECS Component: Test1
//##        Used to test ecs
//############################
struct Test1 {
	int  			test1   { 0 };
    bool 			test2   { false };

	REFLECT()
};


//####################################################################################
//##    Register Reflection / Meta Data
//############################
#ifdef REGISTER_REFLECTION
	REFLECT_STRUCT(Test1)
	REFLECT_MEMBER(test1)
	REFLECT_MEMBER(test2)
	REFLECT_END(Test1)
#endif

#endif	// DR_COMP_TEST1_H

