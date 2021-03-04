//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DRPROJECT_H
#define DRPROJECT_H

#include "project/constants_entity_keys.h"
#include "project/settings/settings.h"


// Project Constants
const int   c_project_width =       800;            // Default Width  for Game
const int   c_project_height =     1600;            // Default Height for Game


enum class Project_Options {        //  Type    User Editable   Description
    Name                = 10,       // string,  yes             Name of Current Project
    File_Name_Path      = 11,       // string,  kind of         Full Path and File Name of Project, will save to this unless choose Save As

    Current_World       = 20,       // long,    no              World currently displayed in Editor_Mode::World_Creator
    Current_Stage       = 21,       // long,    no              Scene currently displayed in Editor_Mode::World_Creator

    Orientation         = 30,       // int,     yes             This Projects target device orientation (enum Orientation)
    Width               = 31,       // long,    yes             This Projects target device window width,  usually 800
    Height              = 32,       // long,    yes             This Projects target device window height, usually 1600

    World_Graph_Center  = 40,       // pointf,  no              Saves center location of Editor_Mode::World_Graph view
    World_Graph_Zoom    = 41,       // double,  no              Saves zoom level of Editor_Mode::World_Graph view

    Current_Mix         = 50,       // long,    no              DrMix currently displayed in Editor_Mode::Sound_Creator

};

enum class Orientation {
    Portrait            = 0,
    Landscape           = 1,
};

// Forward Declarations
class DrAnimation;
class DrAsset;
class DrBlock;
class DrDevice;
class DrEffect;
class DrFont;
class DrImage;
class DrItem;
class DrPrefab;
class DrMix;
class DrSound;
class DrStage;
class DrThing;
class DrWorld;
class IProgressBar;
namespace SoLoud { class AudioSource; }

// Type Definitions
typedef std::map<Project_Options,   DrVariant>      OptionMap;

// Entity Type Definitions
typedef std::map<long,              DrAnimation*>   AnimationMap;
typedef std::map<long,              DrAsset*>       AssetMap;
typedef std::map<long,              DrBlock*>       BlockMap;
typedef std::map<long,              DrDevice*>      DeviceMap;
typedef std::map<long,              DrEffect*>      EffectMap;
typedef std::map<long,              DrFont*>        FontMap;
typedef std::map<long,              DrImage*>       ImageMap;
typedef std::map<long,              DrItem*>        ItemMap;
typedef std::map<long,              DrPrefab*>      PrefabMap;
typedef std::map<long,              DrMix*>         MixMap;
typedef std::map<long,              DrSound*>       SoundMap;
typedef std::map<long,              DrWorld*>       WorldMap;


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
    // Usage Variables
    bool            m_has_saved             { false };              // If project is edited at all, turns to false until user saves
    bool            m_test_only             { false };              // For debugging purposes, stops Drop from asking to save when exiting
    bool            m_world_graph_shown     { false };              // When a new project is loaded, this flag tells World Graph to recenter the first time it is shown

    // Project Variables
    long            m_key_generator     { c_starting_key_entity };  // Variable to hand out unique id key's to all children entities
    OptionMap       m_options;                                      // Map holding DrProject Wide options


    // **********
    //      Every Item In These Project Maps is called an Entity and:
    //          - Inherit DrSettings for DrComponent / DrProperty usage
    //          - Is assigned an unique key upon creation from DrProject::getNextKey()
    //
    // World Items
    WorldMap        m_worlds;                                       // Holds DrWorlds       (which in turn hold DrStages, which hold DrThings)

    // Shared Items
    AnimationMap    m_animations;                                   // Holds DrAnimations   (which in turn hold DrFrames)
    FontMap         m_fonts;                                        // Holds DrFonts        (currently custom bitmap fonts)
    ImageMap        m_images;                                       // Holds DrImages       (for use in DrFrames, loaded into DrEngineTextures)
    MixMap          m_mixes;                                        // Holds DrMix          (which in turn hold a collection of DrTracks)
    SoundMap        m_sounds;                                       // Holds DrSounds       (hold sound files / effects)

    // World Graph Items
    BlockMap        m_blocks;                                       // Holds DrBlocks       (used as building blocks for World Graph nodes)

    // Physics World 2D Items
    AssetMap        m_assets;                                       // Holds DrAssets       (used as building blocks for DrThings)
    DeviceMap       m_devices;                                      // Holds DrDevices      (used as building blocks for DrThings)
    EffectMap       m_effects;                                      // Holds DrEffects      (used as building blocks for DrThings)
    ItemMap         m_items;                                        // Holds DrItems        (used as building blocks for DrThings)
    PrefabMap       m_prefabs;                                      // Holds DrPrefabs      (used as blue prints for multiple types of pre-designed DrThings)


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:



    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    bool        hasSaved()                          { return m_has_saved; }
    bool        isTestOnly()                        { return m_test_only; }
    bool        worldGraphShown()                   { return m_world_graph_shown; }
    void        setHasSaved(bool saved)             { m_has_saved = saved; }
    void        setIsTestOnly(bool test)            { m_test_only = test; }
    void        setWorldGraphShown(bool loaded)     { m_world_graph_shown = loaded; }


    // Entity Key Generator
    long            checkCurrentGeneratorKey()                      { return m_key_generator; }
    long            getNextKey()                                    { return m_key_generator++; }
    void            setGeneratorKeyStartNumber(long initial_key)    { m_key_generator = initial_key; }


    // Options Calls
    DrVariant       getOption(Project_Options option_to_get)                        { return m_options[option_to_get]; }
    void            setOption(Project_Options option_to_set, DrVariant new_value)   { m_options[option_to_set] = new_value; }

    Orientation     getOptionOrientation() { return static_cast<Orientation>( m_options[Project_Options::Orientation].toInt()); }


    // Finding Project Entities
    ImageMap&       getImageMap()           { return m_images; }
    WorldMap&       getWorldMap()           { return m_worlds; }

    std::set<long>  getImageKeysUsedByProject();

    DrSettings*     findSettingsFromKey(long check_key, bool show_warning = true, std::string custom_error = "");
    DrSettings*     findSettingsFromKeyOfType(long check_key, DrType type);
    DrType          findSettingsTypeFromKey(long check_key);

    DrImage*        findImageFromKey(long check_key);
    DrStage*        findStageFromKey(long check_key, long world_key = c_no_key);
    DrThing*        findThingFromKey(long check_key, long stage_key = c_no_key, long world_key = c_no_key);
    DrWorld*        findWorldFromKey(long check_key);


    // Worlds
    long            getFirstWorldKey()      { return m_worlds.begin()->first; }
    long            getNumberOfWorlds()     { return static_cast<long>(m_worlds.size()); }
    DrWorld*        findWorldWithName(std::string world_name);

    // Children Creation Calls
    DrImage*        addImage(std::string image_name, DrBitmap &bitmap, long key = c_no_key, bool outline = true, IProgressBar *progress = nullptr);
    DrWorld*        addWorld(DrWorldType world_type);
    DrWorld*        addWorld(DrWorldType world_type, long key, long start_stage_key, long last_stage_in_editor_key);
    DrWorld*        addWorldCopyFromWorld(DrWorld* from_world, std::string new_name);


    // Project Building
    void            clearProject(bool add_built_in_items = true);
    void            deleteEntity(long entity_key);
    void            deleteWorld(long world_key);

};

#endif // DRPROJECT_H


