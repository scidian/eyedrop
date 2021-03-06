//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "../Editor.h"
#include "Dockspace.h"


//####################################################################################
//##    Dockspace Handler
//####################################################################################
void DockspaceUI(bool &open) {
    // Create main background window for DockSpace
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.f, 1.f));
    ImGui::Begin("DockSpaceViewport", &open, window_flags);
    ImGui::PopStyleVar(3);
            
    // Programatically build dock space
    ImGuiID dockspace_id = ImGui::GetID(DOCKSPACE_NAME);
    if (ImGui::DockBuilderGetNode(dockspace_id) == NULL) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::DockBuilderRemoveNode(dockspace_id);                                                             // Clear out existing layout

        ImGuiDockNodeFlags node_flags = ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode | 
                                        ImGuiDockNodeFlags_NoDockingInCentralNode;
        ImGui::DockBuilderAddNode(dockspace_id, node_flags);                                                    // Add empty node
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);           

        ImGuiID dock_main_id =      dockspace_id; 
        ImGuiID dock_id_top =       ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up,    0.001f, NULL, &dock_main_id);
        ImGuiID dock_id_bottom =    ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down,  0.001f, NULL, &dock_main_id);
        ImGuiID dock_id_left =      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left,  0.150f, NULL, &dock_main_id);
        ImGuiID dock_id_right =     ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.150f, NULL, &dock_main_id);
        
        ImGui::DockBuilderDockWindow("Toolbar",             dock_id_top);               // ---> To apply to central node: "dock_main_id"
        ImGui::DockBuilderDockWindow("Status Bar",          dock_id_bottom);
        ImGui::DockBuilderDockWindow("Assets",              dock_id_left);
        ImGui::DockBuilderDockWindow("Property Inspector",  dock_id_right);
        
        ImGuiDockNode* Node;
        // Main
        Node = ImGui::DockBuilderGetNode( dockspace_id );
        Node->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;
        // Toolbar
        Node = ImGui::DockBuilderGetNode( dock_id_top );
        Node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoDockingSplitMe | 
                            ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoResizeY;
        // Status
        Node = ImGui::DockBuilderGetNode( dock_id_bottom );
        Node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoDockingSplitMe | 
                            ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoResizeY;
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



