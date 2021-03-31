//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "editor/Editor.h"
#include "Menu.h"


//####################################################################################
//##    Main Menu
//####################################################################################
void MainMenuUI(bool* widgets) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            bool clicked_new  { false };
            bool clicked_open { false };
            bool clicked_save { false };
            if (ImGui::MenuItem("New",  "CTRL+N", &clicked_new)) { }
            if (ImGui::MenuItem("Open", "CTRL+O", &clicked_open)) { }
            if (ImGui::MenuItem("Save", "CTRL+S", &clicked_save)) { }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "CTRL+Q")) { sapp_request_quit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Cut", "CTRL+X")) { }
            if (ImGui::MenuItem("Copy", "CTRL+C")) { }
            if (ImGui::MenuItem("Paste", "CTRL+V")) { }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Asset Viewer", 0,          &widgets[EDITOR_WIDGET_ASSETS]);
            ImGui::MenuItem("Object Inspector", 0,      &widgets[EDITOR_WIDGET_INSPECTOR]);
            ImGui::Separator();
            ImGui::MenuItem("Color Theme Selector", 0,  &widgets[EDITOR_WIDGET_THEME]);
            ImGui::MenuItem("Style Selector", 0,        &widgets[EDITOR_WIDGET_STYLE]);
            ImGui::Separator();
            ImGui::MenuItem("ImGui Demo", 0,            &widgets[EDITOR_WIDGET_DEMO]);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}

