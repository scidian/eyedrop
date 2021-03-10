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
#include "Types.h"


//####################################################################################
//##    DrGame
//##        Class to hold data for current active Game
//############################
class DrGame
{
public:
    // Constructor / Destructor
    DrGame();
    ~DrGame();


    // #################### VARIABLES ####################
private:
    // Game Variables
    long            m_key_generator         { KEY_START };                  // Variable to hand out unique id key's to all Game::Entities

    // Scene Variables
    long            m_current_scene         { KEY_NONE };                   // Scene currently displayed in Editor_Mode::World_Creator
    //Entities        m_entities;                                             // Holds all the game's Entities
    //EntityLists     m_lists;                                                // Holds lists of Archetypes of available Entities in the game



    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:
    // Key Generator
    long                checkCurrentGeneratorKey()                      { return m_key_generator; }
    long                getNextKey()                                    { return m_key_generator++; }
    void                setGeneratorKeyStartNumber(long initial_key)    { m_key_generator = initial_key; }
    
};

#endif // DR_GAME_H


