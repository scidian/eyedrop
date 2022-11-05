/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
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