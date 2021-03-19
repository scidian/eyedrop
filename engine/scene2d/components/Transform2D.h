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
	std::vector<double>		position  		{ };
	DrVec3 					rotation		{ };
	std::vector<double>		scale_xyz		{ };

	REFLECT();
};


//####################################################################################
//##    Register Reflection / Meta Data
//############################
#ifdef REGISTER_REFLECTION
	REFLECT_STRUCT(Transform2D)
		STRUCT_META_TITLE("Transform 2D")
		STRUCT_META_DESCRIPTION("Describes the location and positioning of this 2D object.")
		STRUCT_META_COLOR(DROP_COLOR_WHITE)
	REFLECT_MEMBER(position)
		MEMBER_META_DESCRIPTION("Location of this object in 2D space.")
		MEMBER_META_TYPE(Property_Type::Point3D)
	REFLECT_MEMBER(rotation)
		MEMBER_META_DESCRIPTION("Rotation of this object in 2D space.")
		MEMBER_META_TYPE(Property_Type::Point3D)
	REFLECT_MEMBER(scale_xyz)
		MEMBER_META_DESCRIPTION("Scaling of this object.")
		MEMBER_META_TYPE(Property_Type::Point3D)
	REFLECT_END(Transform2D)
#endif


#endif	// DR_COMP_TRANSFORM2D_H

