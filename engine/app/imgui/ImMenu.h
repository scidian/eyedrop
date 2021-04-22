//####################################################################################
//
// Description:     ImMenu, ImGui to MacOS Main Menu Bar Wrapper
// Author:          Stephens Nunnally and Scidian Software
// License:         Distributed under the MIT License
// Source(s):       https://github.com/stevinz/immenu
// Original Idea:   https://github.com/JamesBoer/ImFrame
//
// Copyright (c) 2021 Stephens Nunnally and Scidian Software
// Copyright (c) 2021 James Boer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//####################################################################################
#include <string>

// If compiling for Apple, MacOS and menu is enabled, define flag that calls osx functions
#if defined(__APPLE__) && defined(__MACH__) && defined(DROP_MAC_MENU)
    #ifndef IMMENU_MAC_MENU
        #define IMMENU_MAC_MENU
    #endif
#endif

//####################################################################################
//##    ImMenu
//##        ImGui wrapper for OS specific menu functions
//############################
namespace ImMenu {

    // Using Mac Menus?
    bool UsingMacMenus();

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
