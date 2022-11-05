/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_GAME_H
#define DR_GAME_H

// Includes
#include "engine/ecs/Coordinator.h"
#include "Keys.h"


//####################################################################################
//##    DrGame
//##        Class to hold data for current active Game
//############################
class DrGame
{
public:
    // Constructor / Destructor
    DrGame(int game_key_start = KEY_START) : m_game_keys(game_key_start) { }
    ~DrGame() { }


    // #################### VARIABLES ####################
private:
    // Scene Variables
    DrKeys          m_game_keys             { };                                    // Key generator for Game Assets
    int             m_current_scene         { KEY_NONE };                           // Scene currently displayed in Editor_Mode::World_Creator
    //Entities        m_entities;                                                   // Holds all the game's Entities
    //EntityLists     m_lists;                                                      // Holds lists of Archetypes of available Entities in the game



    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:


};

#endif // DR_GAME_H


