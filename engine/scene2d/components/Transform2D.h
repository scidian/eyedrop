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
#include "engine/data/Meta.h"


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
//##    Register Meta Data
//############################
#ifdef REGISTER_META_DATA
	void RegisterComponentTransform2D() {
		RegisterMetaComponent<Transform2D>( 
			"Transform 2D", 
			"Describes the location and positioning of this 2D object." 
		);
		RegisterMetaProperty<Transform2D>(
			"Position", 
			"Location of this object in 2D space.",
			offsetof(Transform2D, position)
		);
		RegisterMetaProperty<Transform2D>(
			"Rotation", 
			"Rotation of this object in 2D space.",
			offsetof(Transform2D, rotation)
		);
	}
#endif

#endif	// DR_COMP_TRANSFORM2D_H

