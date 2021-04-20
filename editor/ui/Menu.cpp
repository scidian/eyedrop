//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "editor/Editor.h"
#include "engine/app/imgui/ImUtilities.h"
#include "Menu.h"


//####################################################################################
//##    Main Menu
//####################################################################################
void MainMenuUI(bool* widgets) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImUtilities::BeginMainMenuBar()) {
        if (ImUtilities::BeginMenu("File")) {
            bool clicked_new  { false };
            bool clicked_open { false };
            bool clicked_save { false };
            if (ImUtilities::MenuItem("New",  "CTRL+N", &clicked_new)) { }
            if (ImUtilities::MenuItem("Open", "CTRL+O", &clicked_open)) { }
            if (ImUtilities::MenuItem("Save", "CTRL+S", &clicked_save)) { }
            #ifndef DROP_TARGET_HTML5
                ImUtilities::Separator();
                if (ImUtilities::MenuItem("Quit", "CTRL+Q")) { sapp_request_quit(); }
            #endif
            ImUtilities::EndMenu();
        }
        if (ImUtilities::BeginMenu("Edit")) {
            if (ImUtilities::MenuItem("Cut", "CTRL+X")) { }
            if (ImUtilities::MenuItem("Copy", "CTRL+C")) { }
            if (ImUtilities::MenuItem("Paste", "CTRL+V")) { }
            ImUtilities::EndMenu();
        }
        if (ImUtilities::BeginMenu("View")) {
            ImUtilities::MenuItem("Asset Viewer", 0,          &widgets[EDITOR_WIDGET_ASSETS]);
            ImUtilities::MenuItem("Object Inspector", 0,      &widgets[EDITOR_WIDGET_INSPECTOR]);
            ImUtilities::Separator();
            ImUtilities::MenuItem("Color Theme Selector", 0,  &widgets[EDITOR_WIDGET_THEME]);
            ImUtilities::MenuItem("Style Selector", 0,        &widgets[EDITOR_WIDGET_STYLE]);
            ImUtilities::Separator();
            ImUtilities::MenuItem("ImGui Demo", 0,            &widgets[EDITOR_WIDGET_DEMO]);
            ImUtilities::EndMenu();
        }
        ImUtilities::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}

