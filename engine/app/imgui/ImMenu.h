//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Apr 19 2021
//
//
#include <string>

// If compiling for Apple, MacOS and menu is enabled, define flag that calls osx functions
#if defined(__APPLE__) && defined(__MACH__) && defined(ENABLE_IMGUI_MAC_MENU)
    #ifndef IMMENU_MENU_MAC
        #define IMMENU_MENU_MAC
    #endif
#endif

//####################################################################################
//##    ImMenu
//##        ImGui wrapper for OS specific menu functions
//############################
namespace ImMenu {

    // Init Functions
    void MenuInitialize(const char* app_name);
    void MenuShutDown();

    // ImGui / Native Menu Wrappers
    bool BeginMainMenuBar();
    void EndMainMenuBar();
    bool BeginMenu(const char* label, bool enabled = true);
    void EndMenu();
    bool MenuItem(const char* label, const char* shortcut, bool selected = false, bool enabled = true);
    bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);
    void Separator();

	// MacOS Menu Functions
    void osxMenuInitialize(const char* app_name);
    void osxMenuShutDown();
    bool osxBeginMainMenuBar();
    void osxEndMainMenuBar();
    bool osxBeginMenu(const char* label, bool enabled);
    void osxEndMenu();
    bool osxMenuItem(const char* label, const char* shortcut, bool selected, bool enabled);
    bool osxMenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled);
    void osxSeparator();

}
