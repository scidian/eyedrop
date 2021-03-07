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
#include "Toolbar.h"


//####################################################################################
//##    Toolbar
//####################################################################################
void ToolbarUI(bool* widgets, ImTextureID* images, int menu_height) {
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
	    	ImGui::ImageButton(images[EDITOR_IMAGE_WORLD_GRAPH], ImVec2(EDITOR_TOOLBAR_SIZE - 10, EDITOR_TOOLBAR_SIZE - 14));
		}
		ImGui::SameLine();
		ImGui::Button("Button 1", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
		ImGui::SameLine();
        ImGui::Button("Button 2", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
  		ImGui::SameLine();
        ImGui::Button("Button 3", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
       
	   	ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f,1.f,0.f,1.f));
		ImGui::Text(ICON_FA_MOON "  Moon");    									// use string literal concatenation
		ImGui::PopStyleColor();
		
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f,0.f,0.f,1.f));
        ImGui::Text(ICON_FA_HOSPITAL "  Hospital ");
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f,1.f,0.f,1.f));
		ImGui::Text(ICON_FA_KISS_WINK_HEART ICON_FA_LAUGH_SQUINT ICON_FA_LIGHTBULB ICON_FA_LIFE_RING);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1.f));
		ImGui::Text(ICON_FA_SAVE ICON_FA_SHARE_SQUARE ICON_FA_SUN);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f,0.2f,0.7f,1.f));
		ImGui::Text(ICON_FA_SUITCASE ICON_FA_THERMOMETER ICON_FA_THUMBS_UP);
		ImGui::PopStyleColor();

		ImGui::SameLine();
		float window_font_scale = ImGui::GetCurrentWindow()->FontWindowScale;
		ImGui::SetWindowFontScale(1.5f);
		float clr = (sapp_frame_count() % 256) * (1.f / 256.f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(clr,0.0f,0.0f,1.f));
		ImGui::Text(ICON_FA_PROJECT_DIAGRAM);
		ImGui::PopStyleColor();
		ImGui::SetWindowFontScale(window_font_scale);

	ImGui::End();
}

