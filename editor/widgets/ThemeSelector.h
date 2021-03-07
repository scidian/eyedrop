//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 05 2021
//
//
#ifndef DR_THEME_SELECTOR_H
#define DR_THEME_SELECTOR_H

#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/imgui/imgui.h"
#include "3rd_party/sokol/sokol_imgui.h"
    
    
//####################################################################################
//##    ImGui Color Theme Selector
//############################
void    ThemeSelectorUI(bool &open, ImGuiWindowFlags flags, bool force);


#endif // DR_THEME_SELECTOR_H
