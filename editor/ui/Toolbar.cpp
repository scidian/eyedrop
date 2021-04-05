//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#include "engine/app/image/Image.h"
#include "editor/Editor.h"


//####################################################################################
//##    Toolbar
//####################################################################################
void ToolbarUI(bool* widgets, std::vector<std::shared_ptr<DrImage>>& images, int menu_height) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menu_height));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, EDITOR_TOOLBAR_SIZE));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoDocking 
		| ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoMove 
		| ImGuiWindowFlags_NoScrollbar 
		| ImGuiWindowFlags_NoSavedSettings;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 3.f));
	ImGui::Begin("TOOLBAR", &widgets[EDITOR_WIDGET_TOOLBAR], window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
  
		if (images[EDITOR_IMAGE_WORLD_GRAPH] != nullptr) {
	    	ImGui::ImageButton(images[EDITOR_IMAGE_WORLD_GRAPH]->imguiID(), ImVec2(EDITOR_TOOLBAR_SIZE - 10, EDITOR_TOOLBAR_SIZE - 14),
				ImVec2(images[EDITOR_IMAGE_WORLD_GRAPH]->uv0().x, images[EDITOR_IMAGE_WORLD_GRAPH]->uv0().y), 
				ImVec2(images[EDITOR_IMAGE_WORLD_GRAPH]->uv1().x, images[EDITOR_IMAGE_WORLD_GRAPH]->uv1().y));
		}

		ImGui::SameLine();
		if (images[EDITOR_IMAGE_WORLD_CREATOR] != nullptr) {
	    	ImGui::ImageButton(images[EDITOR_IMAGE_WORLD_CREATOR]->imguiID(), ImVec2(EDITOR_TOOLBAR_SIZE - 10, EDITOR_TOOLBAR_SIZE - 14),
				ImVec2(images[EDITOR_IMAGE_WORLD_CREATOR]->uv0().x, images[EDITOR_IMAGE_WORLD_CREATOR]->uv0().y), 
				ImVec2(images[EDITOR_IMAGE_WORLD_CREATOR]->uv1().x, images[EDITOR_IMAGE_WORLD_CREATOR]->uv1().y));
		}
		
		ImGui::SameLine();
		if (images[EDITOR_IMAGE_UI_CREATOR] != nullptr) {
	    	ImGui::ImageButton(images[EDITOR_IMAGE_UI_CREATOR]->imguiID(), ImVec2(EDITOR_TOOLBAR_SIZE - 10, EDITOR_TOOLBAR_SIZE - 14),
				ImVec2(images[EDITOR_IMAGE_UI_CREATOR]->uv0().x, images[EDITOR_IMAGE_UI_CREATOR]->uv0().y), 
				ImVec2(images[EDITOR_IMAGE_UI_CREATOR]->uv1().x, images[EDITOR_IMAGE_UI_CREATOR]->uv1().y));
		}

		ImGui::SameLine();
		ImGui::Button("Button 1", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
		ImGui::SameLine();
        ImGui::Button("Button 2", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
  		ImGui::SameLine();
        ImGui::Button("Button 3", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));

	ImGui::End();
}

