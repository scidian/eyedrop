//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Apr 19 2021
//
//
#include "3rd_party/imgui/imgui.h"
#include "ImUtilities.h"

//####################################################################################
//##    ImUtilities
//##        ImGui wrapper for OS specific functions
//############################
namespace ImUtilities {
	
	// Global OS Variable
    static Utilities_Target s_target_os { UTILITIES_TARGET_OTHER };

	// Sets target operating system
	void TargetOS(Utilities_Target target_os) {
		s_target_os = target_os;
	}

	// Needs to be called before any specific menu functions
 	void MenuInitialize(const char* app_name) {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
			osxMenuInitialize(app_name);
		}
    }

	// Needs to be called during App closure
	void MenuShutDown() {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
			osxMenuShutDown();
		}
    }

	// Replacement for ImGui::BeginMainMenuBar()
    bool BeginMainMenuBar() {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
        	return osxBeginMainMenuBar();
		} else {
        	return ImGui::BeginMainMenuBar();
		}
    }

	// Replacement for ImGui::EndMainMenuBar()
    void EndMainMenuBar() {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
			osxEndMainMenuBar();
		} else {
		    ImGui::EndMainMenuBar();
		}
    }

	// Replacement for ImGui::BeginMenu()
    bool BeginMenu(const char* label, bool enabled) {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
        	return osxBeginMenu(label, enabled);
		} else {
        	return ImGui::BeginMenu(label, enabled);
		}
    }

	// Replacement for ImGui::EndMenu()
    void EndMenu() {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
        	osxEndMenu();
		} else {
        	ImGui::EndMenu();
		}
    }

	// Replacement for ImGui::MenuItem()
    bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
        	return osxMenuItem(label, shortcut, selected, enabled);
		} else {
        	return ImGui::MenuItem(label, shortcut, selected, enabled);
		}
    }

	// Replacement for ImGui::MenuItem()
    bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled) {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
			return osxMenuItem(label, shortcut, p_selected, enabled);
		} else {
			return ImGui::MenuItem(label, shortcut, p_selected, enabled);
		}
    }

	// Replacement for ImGui::Separator()
    void Separator() {
		if (s_target_os == UTILITIES_TARGET_MACOS) {
			osxSeparator();
		} else {
				ImGui::Separator();
		}
    }

}
