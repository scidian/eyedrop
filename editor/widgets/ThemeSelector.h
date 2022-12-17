/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_THEME_SELECTOR_H
#define DR_THEME_SELECTOR_H

#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/imgui/imgui.h"
#include "3rd_party/sokol/sokol_imgui.h"


//####################################################################################
//##    ImGui Color Theme Selector
//############################
void    ThemeSelectorUI(bool& open, ImGuiWindowFlags flags, bool force);


#endif // DR_THEME_SELECTOR_H

