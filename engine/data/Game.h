//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 05 2021
//
//
#ifndef DR_GAME_H
#define DR_GAME_H

// Includes
#include "engine/ecs/Coordinator.h"
#include "Keys.h"


//####################################################################################
//##    DrGame
//##        Class to hold data for current active Game
//############################
class DrGame : public DrKeys
{
public:
    // Constructor / Destructor
    DrGame(std::vector<int> key_starts = { }) : DrKeys(KEY_SINGLE_GENERATOR, key_starts) { }
    ~DrGame() { } 


    // #################### VARIABLES ####################
private:
    // Scene Variables
    int             m_current_scene         { KEY_NONE };                           // Scene currently displayed in Editor_Mode::World_Creator
    //Entities        m_entities;                                                   // Holds all the game's Entities
    //EntityLists     m_lists;                                                      // Holds lists of Archetypes of available Entities in the game



    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:

    
};

#endif // DR_GAME_H


