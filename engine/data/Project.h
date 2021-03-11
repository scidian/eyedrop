//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
//
//####################################################################################
//                                                      App Hierarchy
//                                           _________________|_________________
//                                          |                                   |
//                                       Project                              Game
//                                          |                                   |
//  Font, Image, Prefab, Shader, Sound, World, Etc... Scenes__________________Scenes
//  |                                                     |         |
//  |________________________Assets_______________________|       Entity
//                              |                                   |
//                         DrComponent----------|-------------ECS Component
//                              |               |                   |
//                         DrProperty           |              ECS Property
//                              |_______________|___________________|
//                                              |
//                                          Meta Data
//
//####################################################################################
#ifndef DR_PROJECT_H
#define DR_PROJECT_H

// Includes
#include "../ecs/Types.h"

// Project Constants
#define DEFAULT_PROJECT_WIDTH            800            // Default Width  for Game
#define DEFAULT_PROJECT_HEIGHT          1600            // Default Height for Game

enum Orientation {
    ORIENTATION_PORTAIT =       0,
    ORIENTATION_LANDSCAPE =     1,
};


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
    long            m_key_generator         { KEY_START };                          // Variable to hand out unique id key's to all Project::Entities
    
    // Project Options
    std::string     m_name                  { "" };                                 // Name of Current Project
    std::string     m_file_path_name        { "" };                                 // Full Path and File Name of Project, will save to this unless choose Save As

    int             m_orientation           { ORIENTATION_PORTAIT };                // This Project's target device orientation (enum Orientation)
    long            m_width                 { DEFAULT_PROJECT_WIDTH };              // This Project's target device window width,  usually 800
    long            m_height                { DEFAULT_PROJECT_HEIGHT };             // This Project's target device window height, usually 1600

    long            m_current_scene         { KEY_NONE };                           // Scene currently displayed in Editor_Mode::World_Creator

    // Project Items
    //Entities        m_entities;                                                     // Holds all the Project's Entities
    //EntityLists     m_lists;                                                        // Holds lists of Archetypes of available Entities in the Project


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:
    // Key Generator
    long                checkCurrentGeneratorKey()                      { return m_key_generator; }
    long                getNextKey()                                    { return m_key_generator++; }
    void                setGeneratorKeyStartNumber(long initial_key)    { m_key_generator = initial_key; }

    // Serialization 
    template<class Archive>
    void serialize(Archive& ar, unsigned int version) {
        ar( m_key_generator,
            m_name, 
            m_file_path_name, 
            m_orientation, 
            m_width, 
            m_height,
            m_current_scene
        );
    }

};

#endif // DR_PROJECT_H


