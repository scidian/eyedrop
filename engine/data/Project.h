//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_PROJECT_H
#define DR_PROJECT_H

#include <map>
#include <set>
#include <string>
#include "Types.h"


// Project Constants
#define DEFAULT_PROJECT_WIDTH            800            // Default Width  for Game
#define DEFAULT_PROJECT_HEIGHT          1600            // Default Height for Game

enum Orientation {
    ORIENTATION_PORTAIT =       0,
    ORIENTATION_LANDSCAPE =     1,
};

// Forward Declarations
class DrImage;                      // Holds images for use in Project
class DrScene;                      // Holds Scenes
class DrWorld;                      // Holds info about separate Worlds (collections of Scenes)

// Project Objects
typedef std::map<long, std::shared_ptr<DrImage>>    ImageMap;
typedef std::map<long, std::shared_ptr<DrScene>>    SceneMap;
typedef std::map<long, std::shared_ptr<DrWorld>>    WorldMap;


//####################################################################################
//##    DrProject
//##        Class to hold data for one Project
//############################
class DrProject
{
public:
    // Constructor / Destructor
    DrProject();
    ~DrProject();


    // #################### VARIABLES ####################
private:
    // Project Variables
    long            m_key_generator         { KEY_START };              // Variable to hand out unique id key's to all DrProject::DrEntities
    
    // Project Options
    std::string     m_name                  { "" };                     // Name of Current DrProject
    std::string     m_file_path_name        { "" };                     // Full Path and File Name of DrProject, will save to this unless choose Save As

    long            m_current_world         { KEY_NONE };               // World currently displayed in Editor_Mode::World_Creator
    long            m_current_scene         { KEY_NONE };               // Scene currently displayed in Editor_Mode::World_Creator

    int             m_orientation           { ORIENTATION_PORTAIT };    // This DrProject's target device orientation (enum Orientation)
    long            m_width                 { DEFAULT_PROJECT_WIDTH };  // This DrProject's target device window width,  usually 800
    long            m_height                { DEFAULT_PROJECT_HEIGHT }; // This DrProject's target device window height, usually 1600

    // World Items
    WorldMap        m_worlds;                                           // Holds DrWorlds       (which hold collections of DrScene references)
    SceneMap        m_scenes;                                           // Holds DrScenes       (parent DrEntity, which in turn hold more DrEntites)

    // Shared Items
    ImageMap        m_images;                                           // Holds DrImages


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:
    // Key Generator
    long                checkCurrentGeneratorKey()                      { return m_key_generator; }
    long                getNextKey()                                    { return m_key_generator++; }
    void                setGeneratorKeyStartNumber(long initial_key)    { m_key_generator = initial_key; }

    // Serialization 
    template <class Archive>
    void serialize( Archive & ar, unsigned int version ) {
        ar( m_key_generator,
            m_name, 
            m_file_path_name, 
            m_current_world, 
            m_current_scene, 
            m_orientation, 
            m_width, 
            m_height 
        );
    }

};

#endif // DR_PROJECT_H


