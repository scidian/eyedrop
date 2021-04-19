//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Apr 19 2021
//
//
#include "3rd_party/imgui/imgui.h"
#include "MainMenu.h"

namespace MainMenu {
	
    bool BeginMainMenuBar() {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
        	return osxBeginMainMenuBar();
		#else
        	return ImGui::BeginMainMenuBar();
		#endif
    }

    void EndMainMenuBar() {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
				osxEndMainMenuBar();
		#else
				ImGui::EndMainMenuBar();
		#endif
    }

    bool BeginMenu(const char* label, bool enabled) {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
        	return osxBeginMenu(label, enabled);
		#else
        	return ImGui::BeginMenu(label, enabled);
		#endif
    }

    void EndMenu() {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
        	osxEndMenu();
		#else
        	ImGui::EndMenu();
		#endif
    }

    bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
        	return osxMenuItem(label, shortcut, selected, enabled);
		#else
        	return ImGui::MenuItem(label, shortcut, selected, enabled);
		#endif
    }

    bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled) {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
			return osxMenuItem(label, shortcut, p_selected, enabled);
		#else
			return ImGui::MenuItem(label, shortcut, p_selected, enabled);
		#endif
    }

    void Separator() {
		#if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
				osxSeparator();
		#else
				ImGui::Separator();
		#endif
    }

}
