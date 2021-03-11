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
#include "engine/data/Meta.h"


//####################################################################################
//##    ECS Component: Transform2D
//##        Used to descibe a location of a 2D object in space
//############################
struct Transform2D {
	DrVec3 	position;
	DrVec3 	rotation;
	DrVec3 	scale;
};


#ifdef REGISTER_ECS_COMPONENT_TRANSFORM2D
	void RegisterMetaData () {
		AddProperty("Locaitron", "This is the location", "#FF0000", Proieprty_Type::Slider, "Tree.png")
	}
#endif


#endif	// DR_COMP_TRANSFORM2D_H