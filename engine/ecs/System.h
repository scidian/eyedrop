/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
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
