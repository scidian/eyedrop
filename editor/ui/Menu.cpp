//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "editor/Editor.h"
#include "engine/app/image/Image.h"
#include "engine/app/imgui/ImMenu.h"
#include "Menu.h"


//####################################################################################
//##    Main Menu
//####################################################################################
void MainMenuUI(bool* widgets) {

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImMenu::BeginMainMenuBar(true)) {
        if (ImMenu::BeginMenu("File")) {
            bool clicked_new  { false };
            bool clicked_open { false };
            bool clicked_save { false };
            
            if (ImMenu::MenuItem("New",  "n", &clicked_new)) { }
            // With Icon:
            /*
            static DrBitmap icon = DrBitmap(drop_icon128_3, sizeof(drop_icon128_3));
            static DrImage img("drop_icon_test", icon);
            if (ImMenu::MenuItem("New",  "n", &clicked_new, true, &img)) { }
            */

            if (ImMenu::MenuItem("Open", "o", &clicked_open)) { }
            if (ImMenu::MenuItem("Save", "s", &clicked_save)) { }
            #if !defined(DROP_TARGET_HTML5) && !(defined(DROP_TARGET_OSX) && defined(DROP_MAC_MENU))
                ImMenu::Separator();
                if (ImMenu::MenuItem("Quit", "q")) { sapp_request_quit(); }
            #endif
            ImMenu::EndMenu();
        }
        if (ImMenu::BeginMenu("Edit")) {
            if (ImMenu::MenuItem("Cut", "x")) { }
            if (ImMenu::MenuItem("Copy", "c")) { }
            if (ImMenu::MenuItem("Paste", "v")) { }
            ImMenu::EndMenu();
        }
        if (ImMenu::BeginMenu("View")) {
            ImMenu::MenuItem("Asset Viewer", 0,          &widgets[EDITOR_WIDGET_ASSETS]);
            ImMenu::MenuItem("Object Inspector", 0,      &widgets[EDITOR_WIDGET_INSPECTOR]);
            ImMenu::Separator();
            ImMenu::MenuItem("Color Theme Selector", 0,  &widgets[EDITOR_WIDGET_THEME]);
            ImMenu::MenuItem("Style Selector", 0,        &widgets[EDITOR_WIDGET_STYLE]);
            ImMenu::Separator();
            ImMenu::MenuItem("ImGui Demo", 0,            &widgets[EDITOR_WIDGET_DEMO]);
            ImMenu::EndMenu();
        }
        ImMenu::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}

