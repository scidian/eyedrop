//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#ifndef DR_EDITOR_TYPES_H
#define DR_EDITOR_TYPES_H

//####################################################################################
//##    Modes for Interface Editor Form
//##        These modes interface with some combination of the
//##        Asset Tree, Advisor, Object Inspector, Toolbar, etc
//############################
enum Editor_Mode {
    EDITOR_MODE_CLEAR = 0,
    EDITOR_MODE_LOADING,

    EDITOR_MODE_WORLD_GRAPH,                        // World Node Graph
    EDITOR_MODE_WORLD_CREATOR_2D,                   // 2D Physics World Editor
    EDITOR_MODE_WORLD_CREATOR_3D,
    EDITOR_MODE_UI_CREATOR,                         // UI Editor
    EDITOR_MODE_SOUND_CREATOR,                      // Sound Mixer

    EDITOR_MODE_ANIMATION_CREATOR,                  // Animation Editor
    EDITOR_MODE_IMAGE_CREATOR,                      // Pixel Editor
    EDITOR_MODE_VISUAL_SCRIPTING,                   // Visual Scripting

    // ----- FUTURE -----
    EDITOR_MODE_STAGE_GRAPH,

    EDITOR_MODE_APP_CREATOR,
    EDITOR_MODE_PUZZLE_CREATOR,
    EDITOR_MODE_TOWER_DEFENSE_CREATOR,
    EDITOR_MODE_CARD_GAME_CREATOR,
};


//####################################################################################
//##    Different widgets in an Editor window, used for cross updating widgets
//############################
enum Editor_Widgets {
    // Main Widgets
    EDITOR_WIDGET_MAIN_WINDOW,
    EDITOR_WIDGET_TOOLBAR,
    EDITOR_WIDGET_STATUS,

    // Docks
    EDITOR_WIDGET_ADVISOR,
    EDITOR_WIDGET_ASSETS,
    EDITOR_WIDGET_INSPECTOR,

    // View
    EDITOR_WIDGET_SCENE_VIEW,
    EDITOR_WIDGET_GAME_VIEW,
        
    // ImGui
    EDITOR_WIDGET_DEMO,
    EDITOR_WIDGET_STYLE,
    EDITOR_WIDGET_THEME,

    // !!!!! DO NOT SET !!!!!
    EDITOR_WIDGET_TOTAL_NUMBER,
};


#endif  // DR_EDITOR_TYPES_H
