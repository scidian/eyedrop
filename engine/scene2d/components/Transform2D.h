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

#include "engine/core/imaging/Color.h"
#include "engine/core/Reflect.h"
#include "engine/data/Types.h"


//####################################################################################
//##    ECS Component: Transform2D
//##        Used to descibe a location of a 2D object in space
//############################
struct Transform2D {
	std::vector<double>		position;
	std::vector<double> 	rotation;
	std::vector<double>		scale_xyz;

	REFLECT();
};


//####################################################################################
//##    Register Reflection / Meta Data
//############################
#ifdef REGISTER_REFLECTION
	REFLECT_CLASS(Transform2D)
		CLASS_META_DATA(META_DATA_DESCRIPTION, "Describes the location and positioning of this 2D object.")
		CLASS_META_DATA(META_DATA_COLOR, DrColor(DROP_COLOR_BLUE).name())
	REFLECT_MEMBER(position)
		MEMBER_META_DATA(META_DATA_DESCRIPTION, "Location of this object in 2D space.")
		MEMBER_META_DATA(META_DATA_TYPE, std::to_string(PROPERTY_TYPE_POINT3D))
	REFLECT_MEMBER(rotation)
		MEMBER_META_DATA(META_DATA_DESCRIPTION, "Rotation of this object in 2D space.")
		MEMBER_META_DATA(META_DATA_TYPE, std::to_string(PROPERTY_TYPE_POINT3D))
	REFLECT_MEMBER(scale_xyz)
		MEMBER_META_DATA(META_DATA_DESCRIPTION, "Scaling of this object.")
		MEMBER_META_DATA(META_DATA_TYPE, std::to_string(PROPERTY_TYPE_POINT3D))
	REFLECT_END(Transform2D)
#endif


#endif	// DR_COMP_TRANSFORM2D_H

