//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_COMP_TRANSFORM2D_H
#define DR_COMP_TRANSFORM2D_H

#include <vector>
#include "core/geometry/Vec3.h"
#include "core/imaging/Color.h"
#include "engine/data/Reflect.h"


//####################################################################################
//##    ECS Component: Transform2D
//##        Used to descibe a location of a 2D object in space
//############################
struct Transform2D {
	std::vector<double>	position  	{ };
	DrVec3 				rotation	{ };
	std::vector<double>	scale		{ };
};


//####################################################################################
//##    Register Reflection / Meta Data
//############################
#ifdef REGISTER_REFLECTION
	template <> 
	void RegisterClass<Transform2D>() {
		RegisterComponent<Transform2D>(ComponentData{ 
			"Transform2D", "Transform 2D", "Describes the location and positioning of this 2D object.",
			DROP_COLOR_WHITE, Component_Icon::None, false  
		});
		RegisterProperty<Transform2D>(PropertyData{
			"position", "Position", "Location of this object in 2D space.",
			Property_Type::Point3D, typeid(Transform2D::position).hash_code(), offsetof(Transform2D, position), sizeof(Transform2D::position), false
		});
		RegisterProperty<Transform2D>(PropertyData{
			"rotation", "Rotation", "Rotation of this object in 2D space.",
			Property_Type::Point3D, typeid(Transform2D::rotation).hash_code(), offsetof(Transform2D, rotation), sizeof(Transform2D::rotation), false
		});
	}
#endif

#endif	// DR_COMP_TRANSFORM2D_H

