//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "../Editor.h"
#include "../Types.h"
#include "Dockspace.h"


//####################################################################################
//##    Dockspace Handler
//####################################################################################
void DockspaceUI(bool* widgets, int& menu_height) {
    // Create main background window for DockSpace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos =  ImVec2(viewport->Pos.x,  viewport->Pos.y  + EDITOR_TOOLBAR_SIZE);
    ImVec2 window_size = ImVec2(viewport->Size.x, viewport->Size.y - EDITOR_TOOLBAR_SIZE);
    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar 
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.f, 1.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);    
    ImGui::Begin("MasterDockspace", &widgets[EDITOR_WIDGET_MAIN_WINDOW], window_flags);
    ImGui::PopStyleVar(3);
            
    // Save off menu bar height for later
	menu_height = ImGui::GetCurrentWindow()->MenuBarHeight();

    // Programatically build dock space
    ImGuiID dockspace_id = ImGui::GetID(DOCKSPACE_NAME);
    if (ImGui::DockBuilderGetNode(dockspace_id) == NULL) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::DockBuilderRemoveNode(dockspace_id);                                                             // Clear out existing layout

        ImGuiDockNodeFlags node_flags = 0
            | ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;
        ImGui::DockBuilderAddNode(dockspace_id, node_flags);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);           

        ImGuiID dock_main_id =      dockspace_id; 
        //ImGuiID dock_id_top =       ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up,    0.001f, NULL, &dock_main_id);
        //ImGuiID dock_id_bottom =    ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down,  0.050f, NULL, &dock_main_id);
        ImGuiID dock_id_left =      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left,  0.175f, NULL, &dock_main_id);
        ImGuiID dock_id_right =     ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.200f, NULL, &dock_main_id);
        
        //ImGui::DockBuilderDockWindow("Toolbar",             dock_id_top);             // ---> To apply to central node: "dock_main_id"
        //ImGui::DockBuilderDockWindow("Status Bar",          dock_id_bottom);
        ImGui::DockBuilderDockWindow("Assets",              dock_id_left);
        ImGui::DockBuilderDockWindow("Property Inspector",  dock_id_right);
        
        ImGuiDockNode* Node;
        // Main
        Node = ImGui::DockBuilderGetNode( dockspace_id );
        Node->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;
        // Old top and bottom toolbar / status bar docks
        /*
            // Toolbar
            Node = ImGui::DockBuilderGetNode( dock_id_top );
            Node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoDockingSplitMe | 
                                ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoResizeY;
            // Status
            Node = ImGui::DockBuilderGetNode( dock_id_bottom );
            Node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoDockingSplitMe | 
                                ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoResizeY;
        */
        // Assets
        Node = ImGui::DockBuilderGetNode( dock_id_left );
        Node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;// | ImGuiDockNodeFlags_NoCloseButton;
        // Inspector
        Node = ImGui::DockBuilderGetNode( dock_id_right );
        Node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;// | ImGuiDockNodeFlags_NoCloseButton;

        ImGui::DockBuilderFinish(dockspace_id);
    }

    // Apply dockspace to main dock space viewport
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;// | ImGuiDockNodeFlags_AutoHideTabBar;
    dockspace_id = ImGui::GetID(DOCKSPACE_NAME);
    ImGui::PushStyleColor(ImGuiCol_ChildBg,         ImVec4(0.f,0.f,0.f,0.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,        ImVec4(0.f,0.f,0.f,0.f));
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg,  ImVec4(0.f,0.f,0.f,0.f));
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::PopStyleColor(3);
    ImGui::End();
}



