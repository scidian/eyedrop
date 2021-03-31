//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_SYSTEM_H
#define DR_ECS_SYSTEM_H

#include <set>
#include "engine/data/Constants.h"


//####################################################################################
//##    DrSystem
//##        Base ECS System class
//############################
class DrSystem
{
    // #################### VARIABLES ####################
public:
	std::set<EntityID>      m_entities;


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
	virtual void init() = 0;                                    // To be called upon initialization
	virtual void update(float dt) = 0;                          // To be called by App every update frame

};

#endif  // DR_ECS_SYSTEM_H
