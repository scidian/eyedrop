//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "editor/Editor.h"
#include "engine/app/imgui/MainMenu.h"
#include "Menu.h"


//####################################################################################
//##    Main Menu
//####################################################################################
void MainMenuUI(bool* widgets) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (MainMenu::BeginMainMenuBar()) {
        // if (MainMenu::BeginMenu("File")) {
        //     bool clicked_new  { false };
        //     bool clicked_open { false };
        //     bool clicked_save { false };
        //     if (MainMenu::MenuItem("New",  "CTRL+N", &clicked_new)) { }
        //     if (MainMenu::MenuItem("Open", "CTRL+O", &clicked_open)) { }
        //     if (MainMenu::MenuItem("Save", "CTRL+S", &clicked_save)) { }
        //     MainMenu::Separator();
        //     if (MainMenu::MenuItem("Quit", "CTRL+Q")) { sapp_request_quit(); }
        //     MainMenu::EndMenu();
        // }
        // if (MainMenu::BeginMenu("Edit")) {
        //     if (MainMenu::MenuItem("Cut", "CTRL+X")) { }
        //     if (MainMenu::MenuItem("Copy", "CTRL+C")) { }
        //     if (MainMenu::MenuItem("Paste", "CTRL+V")) { }
        //     MainMenu::EndMenu();
        // }
        if (MainMenu::BeginMenu("View")) {
            MainMenu::MenuItem("Asset Viewer", 0,          &widgets[EDITOR_WIDGET_ASSETS]);
            MainMenu::MenuItem("Object Inspector", 0,      &widgets[EDITOR_WIDGET_INSPECTOR]);
            MainMenu::Separator();
            MainMenu::MenuItem("Color Theme Selector", 0,  &widgets[EDITOR_WIDGET_THEME]);
            MainMenu::MenuItem("Style Selector", 0,        &widgets[EDITOR_WIDGET_STYLE]);
            MainMenu::Separator();
            MainMenu::MenuItem("ImGui Demo", 0,            &widgets[EDITOR_WIDGET_DEMO]);
            MainMenu::EndMenu();
        }
        MainMenu::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}

