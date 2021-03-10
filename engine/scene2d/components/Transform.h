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


//####################################################################################
//##    Component: Transform2D
//##        Used to descibe a location of a 2D object in space
//############################
struct Transform2D {
	Vec3 	position;
	Vec3 	rotation;
	Vec3 	scale;
};


#endif	// DR_COMP_TRANSFORM2D_H