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
#include "3rd_party/imgui/imgui.h"
#include "ImMenu.h"

//####################################################################################
//##    ImMenu
//##        ImGui wrapper for OS specific functions
//####################################################################################
namespace ImMenu {
	
	// Returns true if using Mac Menus
	bool UsingMacMenus() {
		#if defined(IMMENU_MAC_MENU)
			return true;
		#else
			return false;
		#endif
	}

	// Needs to be called before any specific menu functions
 	void MenuInitialize(const char* app_name) {
		#if defined(__APPLE__) && defined(__MACH__)
			osxMenuInitialize(app_name);
		#endif
    }

	// Needs to be called during App closure
	void MenuShutDown() {
		#if defined(__APPLE__) && defined(__MACH__)
			osxMenuShutDown();
		#endif
    }

	// Replacement for ImGui::BeginMainMenuBar()
    bool BeginMainMenuBar(bool first_call) {
		#if defined(IMMENU_MAC_MENU)
        	return osxBeginMainMenuBar(first_call);
		#else
        	return ImGui::BeginMainMenuBar();
		#endif
    }

	// Replacement for ImGui::EndMainMenuBar()
    void EndMainMenuBar() {
		#if defined(IMMENU_MAC_MENU)
			osxEndMainMenuBar();
		#else
		    ImGui::EndMainMenuBar();
		#endif
    }

	// Replacement for ImGui::BeginMenu()
    bool BeginMenu(const char* label, bool enabled) {
		#if defined(IMMENU_MAC_MENU)
        	return osxBeginMenu(label, enabled);
		#else
        	return ImGui::BeginMenu(label, enabled);
		#endif
    }

	// Replacement for ImGui::EndMenu()
    void EndMenu() {
		#if defined(IMMENU_MAC_MENU)
        	osxEndMenu();
		#else
        	ImGui::EndMenu();
		#endif
    }

	// Replacement for ImGui::MenuItem()
    bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled, DrImage* image) {
		#if defined(IMMENU_MAC_MENU)
        	return osxMenuItem(label, shortcut, selected, enabled, image);
		#else
        	return ImGui::MenuItem(label, shortcut, selected, enabled);
		#endif
    }

	// Replacement for ImGui::MenuItem()
    bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled, DrImage* image) {
		#if defined(IMMENU_MAC_MENU)
			return osxMenuItem(label, shortcut, p_selected, enabled, image);
		#else
			return ImGui::MenuItem(label, shortcut, p_selected, enabled);
		#endif
    }

	// Replacement for ImGui::Separator()
    void Separator() {
		#if defined(IMMENU_MAC_MENU)
			osxSeparator();
		#else
			ImGui::Separator();
		#endif
    }

}
