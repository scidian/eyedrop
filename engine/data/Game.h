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

#include <map>
#include <string>
#include "Types.h"

// Forward Declarations
class DrScene;                      // Holds Scenes
class DrWorld;                      // Holds info about separate Worlds (collections of Scenes)

// Game Objects
typedef std::map<long, std::shared_ptr<DrScene>>    SceneMap;
typedef std::map<long, std::shared_ptr<DrWorld>>    WorldMap;


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
    long            m_key_generator         { KEY_START };              // Variable to hand out unique id key's to all DrGame::DrEntities

    // Scene Variables
    long            m_current_scene         { KEY_NONE };               // Scene currently displayed in Editor_Mode::World_Creator
    WorldMap        m_worlds;                                           // Holds DrWorlds       (which hold collections of DrScene references)
    SceneMap        m_scenes;                                           // Holds DrScenes       (parent DrEntity, which in turn hold more DrEntites)


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

