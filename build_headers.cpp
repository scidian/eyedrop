/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/

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
#if defined(DROP_IMGUI)
    #include "3rd_party/imgui/imgui.h"
    #define SOKOL_IMGUI_IMPL
    #include "3rd_party/sokol/sokol_imgui.h"
#endif

// ##### Debug Menu
#if defined(DROP_DEBUG)
    #define SOKOL_GFX_IMGUI_IMPL
    #include "3rd_party/sokol/sokol_gfx_imgui.h"
#endif



