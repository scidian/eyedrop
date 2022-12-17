/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/

//####################################################################################
//                                                      App Hierarchy                   --------- "Dr" prefix (Drop)
//                                           _________________|_________________
//                                          |                                   |
//                                       Project                              Game
//                                          |                                   |
//  Font, Image, Prefab, Shader, Sound, World, Etc... Scenes__________________Scenes    --------- "Ga" prefix (Game)
//  |                                                    |          |
//  |________________________Assets______________________|       Entities
//                              |                                   |                   ____
//                      Component Structs*-------|---------ECS Component Structs*           |
//                                               |                                          |---- Reflection / Meta Data
//                                   Properties (Member Variables)                      ____|
//
//
//  #NOTE: *Component structs / classes MUST be aggregate (std::is_standard_layout) for reflection system.
//          Identifier offsetof() is only guaranteed to work with standard layout classes...
//####################################################################################
#ifndef DR_PROJECT_H
#define DR_PROJECT_H

// Includes
#include <string>
#include <unordered_map>
#include "Keys.h"

// Forward Declarations
class GaAsset;

// Project Constants
#define DEFAULT_PROJECT_WIDTH            800                                        // Default Width  for Game
#define DEFAULT_PROJECT_HEIGHT          1600                                        // Default Height for Game

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
    DrProject(int project_key_start = KEY_START) : m_project_keys(project_key_start) { }
    ~DrProject() { }


    // #################### VARIABLES ####################
private:
    // Project Variables
    DrKeys          m_project_keys          { };                                    // Key generator for Project Assets

    // Project Options
    std::string     m_name                  { "" };                                 // Name of Current Project
    std::string     m_file_path_name        { "" };                                 // Full Path and File Name of Project, will save to this unless choose Save As

    int             m_orientation           { ORIENTATION_PORTAIT };                // This Project's target device orientation (enum Orientation)
    int             m_width                 { DEFAULT_PROJECT_WIDTH };              // This Project's target device window width,  usually 800
    int             m_height                { DEFAULT_PROJECT_HEIGHT };             // This Project's target device window height, usually 1600

    int             m_current_scene         { KEY_NONE };                           // Scene currently displayed in Editor_Mode::World_Creator

    // Project Items
    std::unordered_map<int, std::shared_ptr<GaAsset>>       m_assets        { };    // Holds Project things (Scenes, Images, Sounds, etc)


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:


};

#endif // DR_PROJECT_H


