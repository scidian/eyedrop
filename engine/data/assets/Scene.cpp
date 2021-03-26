//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#include "engine/ecs/Coordinator.h"
#include "Scene.h"


//####################################################################################
//##    Constructor / Destructor 
//####################################################################################
IScene::IScene() {
    // Initialize Entity Component System
    m_ecs = new DrCoordinator();        

}

IScene::~IScene() {
    delete m_ecs;
}