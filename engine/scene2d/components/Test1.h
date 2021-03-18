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
	int  test1   { };
    bool test2   { };
};


//####################################################################################
//##    Register Reflection / Meta Data
//############################
#ifdef REGISTER_REFLECTION
	template <> 
	void RegisterClass<Test1>() {
		RegisterComponent<Test1>(ComponentData{ 
			"Test1", "Test Number 1", "Component used to test ecs.",
			false, DROP_COLOR_WHITE, Component_Icon::None, typeid(Test1).hash_code()
		});
		RegisterProperty<Test1>(PropertyData{
			"test1", "Test Variable 1", "First variable used to test ecs.",
			false, Property_Type::Int, typeid(Test1::test1).hash_code(), offsetof(Test1, test1), sizeof(Test1::test1)
		});
		RegisterProperty<Test1>(PropertyData{
			"test2", "Test Variable 2", "Second variable used to test ecs.",
			false, Property_Type::Bool, typeid(Test1::test2).hash_code(), offsetof(Test1, test2), sizeof(Test1::test2)
		});
	}
#endif

#endif	// DR_COMP_TEST1_H

