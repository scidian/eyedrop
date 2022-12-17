/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "engine/app/core/Math.h"
#include "ThemeSelector.h"


//####################################################################################
//##    ImGui Custom Styler
//####################################################################################
static ImVec4   base = ImVec4(0.000f, 0.650f, 0.620f, 1.0f);
static ImVec4   bg   = ImVec4(0.150f, 0.152f, 0.151f, 1.0f);
static ImVec4   text = ImVec4(0.980f, 1.000f, 0.990f, 1.0f);
static float    high_val =         0.05f;
static float    mid_val =          0.00f;
static float    low_val =         -0.05f;

ImVec4 adjust_color(ImVec4 start, float adjust, float alpha) {
    ImVec4 res(0, 0, 0, alpha);
    ImGui::ColorConvertRGBtoHSV(start.x, start.y, start.z, res.x, res.y, res.z);
    res.z = Clamp(res.z + adjust, 0.f, 1.f);
    ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
    return res;
}

// Shows Selector Widget
void ThemeSelectorUI(bool& open, ImGuiWindowFlags flags, bool force) {
    if ((open == false) && (force == false)) return;

    ImGui::Begin("Style Selector", &open, flags);
    ImGui::ColorEdit3("base",   (float*) &base, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::ColorEdit3("bg",     (float*) &bg,   ImGuiColorEditFlags_PickerHueWheel);
    ImGui::ColorEdit3("text",   (float*) &text, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::SliderFloat("high",  &high_val, -0.5, 0.5);
    ImGui::SliderFloat("mid",   &mid_val,  -0.5, 0.5);
    ImGui::SliderFloat("low",   &low_val,  -0.5, 0.5);

    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                 = adjust_color(text,  0.0f,     1.00f);
    style.Colors[ImGuiCol_TextDisabled]         = adjust_color(text, -0.3f,     0.80f);
    style.Colors[ImGuiCol_WindowBg]             = adjust_color(bg,    low_val,  1.00f);
    style.Colors[ImGuiCol_ChildBg]              = adjust_color(bg,    low_val,  1.00f);
    style.Colors[ImGuiCol_PopupBg]              = adjust_color(bg,    low_val,  0.95f);
    style.Colors[ImGuiCol_Border]               = adjust_color(bg,   -0.4f,     0.75f);
    style.Colors[ImGuiCol_BorderShadow]         = adjust_color(bg,   -0.8f,     0.05f);
    style.Colors[ImGuiCol_FrameBg]              = adjust_color(bg,    0.0f,     1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]       = adjust_color(base,  mid_val,  0.78f);
    style.Colors[ImGuiCol_FrameBgActive]        = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_TitleBg]              = adjust_color(base,  low_val,  1.00f);
    style.Colors[ImGuiCol_TitleBgActive]        = adjust_color(base,  high_val, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]     = adjust_color(bg,    0.0f,     0.75f);
    style.Colors[ImGuiCol_MenuBarBg]            = adjust_color(bg,    0.0f,     0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]          = adjust_color(bg,    0.0f,     1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]        = adjust_color(base,  low_val,  1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = adjust_color(base,  mid_val,  0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_CheckMark]            = adjust_color(base,  high_val, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]           = adjust_color(bg,    0.2f,     1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]     = adjust_color(base, -0.2f,     1.00f);
    style.Colors[ImGuiCol_Button]               = adjust_color(bg,    0.0f,     0.20f);
    style.Colors[ImGuiCol_ButtonHovered]        = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_ButtonActive]         = adjust_color(base,  high_val, 1.00f);
    style.Colors[ImGuiCol_Header]               = adjust_color(base,  mid_val,  0.76f);
    style.Colors[ImGuiCol_HeaderHovered]        = adjust_color(base,  mid_val,  0.86f);
    style.Colors[ImGuiCol_HeaderActive]         = adjust_color(base,  high_val, 1.00f);
    style.Colors[ImGuiCol_Separator]            = adjust_color(bg,   -0.025f,   1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]     = adjust_color(base,  low_val,  1.00f);
    style.Colors[ImGuiCol_SeparatorActive]      = adjust_color(base,  high_val, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]           = adjust_color(bg,    0.025f,   1.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]    = adjust_color(base,  mid_val,  0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]     = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_PlotLines]            = adjust_color(text,  0.0f,     0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]     = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_PlotHistogram]        = adjust_color(text,  0.0f,     0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]       = adjust_color(base,  mid_val,  0.43f);
    style.Colors[ImGuiCol_ModalWindowDimBg]     = adjust_color(bg,    0.0f,     0.73f);
    style.Colors[ImGuiCol_Tab]                  = adjust_color(bg,    0.0f,     0.40f);
    style.Colors[ImGuiCol_TabHovered]           = adjust_color(base,  high_val, 1.00f);
    style.Colors[ImGuiCol_TabActive]            = adjust_color(base,  mid_val,  1.00f);
    style.Colors[ImGuiCol_TabUnfocused]         = adjust_color(bg,    0.0f,     0.40f);
    style.Colors[ImGuiCol_TabUnfocusedActive]   = adjust_color(bg,    0.0f,     0.70f);
    style.Colors[ImGuiCol_DockingPreview]       = adjust_color(base,  high_val, 0.30f);
    style.Colors[ImGuiCol_DockingEmptyBg]       = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (ImGui::Button("Export")) {
        ImGui::LogToTTY();
        ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;\n");
        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            const ImVec4& col = style.Colors[i];
            const char* name = ImGui::GetStyleColorName(i);
            ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n",
                           name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
        }
        ImGui::LogFinish();
    }
    ImGui::End();
}



