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
enum class Editor_Mode {
    Clear                       = 0,
    Program_Loading             = 1,

    World_Graph                 = 2,                        // World Node Graph
    World_Creator_2D            = 3,                        // 2D Physics World Editor
    World_Creator_3D            = 4,
    UI_Creator                  = 5,                        // UI Editor
    Sound_Creator               = 6,                        // Sound Mixer

    Animation_Creator           = 7,                        // Animation Editor
    Image_Creator               = 8,                        // Pixel Editor
    Component_Graph             = 9,                        // Visual Scripting

    // Stage_Graph              = ,

    // App_Creator              = ,
    // Puzzle_World_Creator     = ,
    // Tower_Defense_Creator    = ,
    // Card_Game_Creator        = ,
};


//####################################################################################
//##    Different widgets in an Editor window, used for cross updating widgets
//############################
enum Editor_Widgets {
    // Global Widgets
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
