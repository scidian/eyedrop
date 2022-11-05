/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "engine/app/image/Image.h"
#include "engine/app/imgui/ImGradient.h"
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
	    	ImGui::ImageButton(images[EDITOR_IMAGE_UI_CREATOR]->imguiID(), ImVec2(EDITOR_TOOLBAR_SIZE - 16, EDITOR_TOOLBAR_SIZE - 14),
				ImVec2(images[EDITOR_IMAGE_UI_CREATOR]->uv0().x, images[EDITOR_IMAGE_UI_CREATOR]->uv0().y),
				ImVec2(images[EDITOR_IMAGE_UI_CREATOR]->uv1().x, images[EDITOR_IMAGE_UI_CREATOR]->uv1().y));
		}

		ImGui::SameLine();
		ImGui::Button("Button 1", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
		ImGui::SameLine();
        ImGui::Button("Button 2", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));
  		ImGui::SameLine();
        ImGui::Button("Button 3", ImVec2(0, EDITOR_TOOLBAR_SIZE - 6));


		ImGradient gradient;
			gradient.getMarks().clear();
			gradient.addMark(0.0f, ImColor(0xA0, 0x79, 0x3D));
			gradient.addMark(0.2f, ImColor(0xAA, 0x83, 0x47));
			gradient.addMark(0.3f, ImColor(0xB4, 0x8D, 0x51));
			gradient.addMark(0.4f, ImColor(0xBE, 0x97, 0x5B));
			gradient.addMark(0.6f, ImColor(0xC8, 0xA1, 0x65));
			gradient.addMark(0.7f, ImColor(0xD2, 0xAB, 0x6F));
			gradient.addMark(0.8f, ImColor(0xDC, 0xB5, 0x79));
			gradient.addMark(1.0f, ImColor(0xE6, 0xBF, 0x83));
		//ImGui::SameLine();
 		//if (ImGui::GradientButton(&gradient)) { }


	ImGui::End();
}

