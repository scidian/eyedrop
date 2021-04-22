//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Apr 19 2021
//
//
#include "3rd_party/imgui/imgui.h"
#include "ImMenu.h"

//####################################################################################
//##    ImMenu
//##        ImGui wrapper for OS specific functions
//####################################################################################
namespace ImMenu {

	// Needs to be called before any specific menu functions
 	void MenuInitialize(const char* app_name) {
		#if defined(IMMENU_MENU_MAC)
			osxMenuInitialize(app_name);
		#endif
    }

	// Needs to be called during App closure
	void MenuShutDown() {
		#if defined(IMMENU_MENU_MAC)
			osxMenuShutDown();
		#endif
    }

	// Replacement for ImGui::BeginMainMenuBar()
    bool BeginMainMenuBar() {
		#if defined(IMMENU_MENU_MAC)
        	return osxBeginMainMenuBar();
		#else
        	return ImGui::BeginMainMenuBar();
		#endif
    }

	// Replacement for ImGui::EndMainMenuBar()
    void EndMainMenuBar() {
		#if defined(IMMENU_MENU_MAC)
			osxEndMainMenuBar();
		#else
		    ImGui::EndMainMenuBar();
		#endif
    }

	// Replacement for ImGui::BeginMenu()
    bool BeginMenu(const char* label, bool enabled) {
		#if defined(IMMENU_MENU_MAC)
        	return osxBeginMenu(label, enabled);
		#else
        	return ImGui::BeginMenu(label, enabled);
		#endif
    }

	// Replacement for ImGui::EndMenu()
    void EndMenu() {
		#if defined(IMMENU_MENU_MAC)
        	osxEndMenu();
		#else
        	ImGui::EndMenu();
		#endif
    }

	// Replacement for ImGui::MenuItem()
    bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
		#if defined(IMMENU_MENU_MAC)
        	return osxMenuItem(label, shortcut, selected, enabled);
		#else
        	return ImGui::MenuItem(label, shortcut, selected, enabled);
		#endif
    }

	// Replacement for ImGui::MenuItem()
    bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled) {
		#if defined(IMMENU_MENU_MAC)
			return osxMenuItem(label, shortcut, p_selected, enabled);
		#else
			return ImGui::MenuItem(label, shortcut, p_selected, enabled);
		#endif
    }

	// Replacement for ImGui::Separator()
    void Separator() {
		#if defined(IMMENU_MENU_MAC)
			osxSeparator();
		#else
			ImGui::Separator();
		#endif
    }

}
