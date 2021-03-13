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

#include "core/geometry/Vec3.h"
#include "engine/data/Types.h"


//####################################################################################
//##    ECS Component: Transform2D
//##        Used to descibe a location of a 2D object in space
//############################
struct Transform2D {
	DrVec3		position  	{ };
	DrVec3 		rotation	{ };
	DrVec3		scale		{ };
};


//####################################################################################
//##    Reflection / Meta Data
//############################
#ifdef REGISTER_REFLECTION
	using namespace rttr;
	RTTR_REGISTRATION {
		registration::class_<Transform2D>("Transform2D")	(metadata(Meta_Comp::Name, "Transform"))
			.property("position",  &Transform2D::position) 	(metadata(Meta_Prop::Name, "Position"))
			.property("rotation",  &Transform2D::rotation) 	(metadata(Meta_Prop::Name, "Rotation"))
			.property("scale",     &Transform2D::scale) 	(metadata(Meta_Prop::Name, "Scale"));
	}
#endif

#endif	// DR_COMP_TRANSFORM2D_H

