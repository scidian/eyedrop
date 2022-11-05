/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_SCENE_H
#define DR_SCENE_H

// Forward Declarations
class DrCoordinator;


//####################################################################################
//##    IScene
//##        Interface for scene containing entities within an entity component system
//############################
class IScene
{
public:
    // Constructor / Destructor
    IScene();
    ~IScene();

    // #################### VARIABLES ####################
private:
    // Local Variables
    DrCoordinator*          m_ecs                   { nullptr };                    // Entity Component System


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    DrCoordinator*          ecs()                   { return m_ecs; }

};

#endif  // DR_SCENE_2D_H