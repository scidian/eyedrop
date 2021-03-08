//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 08 2021
//
//
//####################################################################################
//##
//##    c++ Single Header Library Initialization
//##
//####################################################################################
// Includes
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/sokol/sokol_gl.h"
#include "3rd_party/sokol/sokol_glue.h"
#include "3rd_party/sokol/sokol_time.h"
#include "3rd_party/sokol/sokol_audio.h"
#include "3rd_party/sokol/sokol_fetch.h"


//####################################################################################
//##    ImGui Implmentation
//####################################################################################
#if defined (ENABLE_IMGUI)
    #include "3rd_party/imgui/imgui.h"
    #define SOKOL_IMGUI_IMPL
    #include "3rd_party/sokol/sokol_imgui.h"
#endif

// ##### Debug Menu
#if defined (ENABLE_DEBUG)
    #define SOKOL_GFX_IMGUI_IMPL
    #include "3rd_party/sokol/sokol_gfx_imgui.h"
#endif



