//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
// ***** Sokol
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/sokol/sokol_gl.h"
#include "3rd_party/sokol/sokol_glue.h"
#include "3rd_party/sokol/sokol_time.h"
#include "3rd_party/sokol/sokol_audio.h"
#include "3rd_party/sokol/sokol_fetch.h"

// ***** Fonts
#include "3rd_party/fontstash.h"
#include "3rd_party/sokol/sokol_fontstash.h"

// ***** ImGui
#include "3rd_party/imgui/imgui.h"
#include "3rd_party/imgui_font.h"
#define SOKOL_IMGUI_IMPL
#include "3rd_party/sokol/sokol_imgui.h"

// ***** Debug Menu
#ifdef DEBUG_ON
    #define SOKOL_GFX_IMGUI_IMPL
    #include "3rd_party/sokol/sokol_gfx_imgui.h"
#endif

// ***** Html5 / File Handling
#ifndef DROP_TARGET_HTML5
    #include "3rd_party/wai/whereami.c"
#else
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#endif

// ***** Engine
#include "core/math.h"
#include "shaders/basic_shader.glsl.h"
#include "App.h"


// static void font_normal_loaded(const sfetch_response_t* response) {
//     if (response->fetched) {
//         state.font_normal = fonsAddFontMem(state.fons, "sans", (unsigned char*)response->buffer_ptr, (int)response->fetched_size,  false);
//     } 
// }


// ***** C callback wrappers for using member methods as callbacks functions
std::function<void()> initCallback;     
std::function<void()> frameCallback;     
std::function<void(const sapp_event* e)> eventCallback;      
std::function<void()> cleanupCallback;    
extern "C" void initWrapper() { initCallback(); }
extern "C" void frameWrapper() { frameCallback(); }
extern "C" void eventWrapper(const sapp_event* e) { eventCallback(e); }
extern "C" void cleanupWrapper() { cleanupCallback(); }


//####################################################################################
//##    Blend Functions
//####################################################################################
// Normal
sg_blend_state (sokol_blend_normal) {
    .enabled =              true,
    .src_factor_rgb =       SG_BLENDFACTOR_ONE,
    .dst_factor_rgb =       SG_BLENDFACTOR_ZERO,
    .op_rgb =               SG_BLENDOP_ADD,
    .src_factor_alpha =     SG_BLENDFACTOR_ONE,
    .dst_factor_alpha =     SG_BLENDFACTOR_ZERO,
    .op_alpha =             SG_BLENDOP_ADD,
};
// Alpha Enabled
sg_blend_state (sokol_blend_alpha) {
    .enabled =              true,
    .src_factor_rgb =       SG_BLENDFACTOR_SRC_ALPHA,
    .dst_factor_rgb =       SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    .op_rgb =               SG_BLENDOP_ADD,
    .src_factor_alpha =     SG_BLENDFACTOR_SRC_ALPHA,
    .dst_factor_alpha =     SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    .op_alpha =             SG_BLENDOP_ADD,
};


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrApp::DrApp(std::string title, DrColor bg_color, int width, int height) {
    m_bg_color = bg_color;

    initCallback =      std::bind(&DrApp::init, this);
    frameCallback =     std::bind(&DrApp::frame, this);
    eventCallback =     std::bind(&DrApp::event, this, std::placeholders::_1);
    cleanupCallback =   std::bind(&DrApp::cleanup, this);
    
    m_sokol_app.window_title =          title.c_str();
    m_sokol_app.init_cb =               initWrapper;    
    m_sokol_app.frame_cb =              frameWrapper;
    m_sokol_app.event_cb =              eventWrapper;
    m_sokol_app.cleanup_cb =            cleanupWrapper;
    m_sokol_app.width =                 width;
    m_sokol_app.height =                height;
    m_sokol_app.enable_clipboard =      true;
    m_sokol_app.enable_dragndrop =      true;
    m_sokol_app.max_dropped_files =     100;
}

DrApp::~DrApp() {
        
}



//####################################################################################
//##    Sokol App Events - Init
//####################################################################################
void DrApp::init(void) {
    // #################### Sokol Gfx ####################
    sg_desc sokol_gfx {};
        sokol_gfx.context = sapp_sgcontext();                   // Call sokol_glue function to auto obtain values from sokol_app      
    sg_setup(&sokol_gfx);

    // #################### Sokol Gl ####################
    sgl_desc_t sokol_gl {};
    sgl_setup(&sokol_gl);

    // #################### Sokol Time ####################
    stm_setup();
    m_time_start = stm_now();

    // #################### Sokol Fetch ####################
    // Used for loading files with the minimal "resource limits"
    sfetch_desc_t sokol_fetch {};
        sokol_fetch.max_requests = 128;
        sokol_fetch.num_channels = 2;
        sokol_fetch.num_lanes =    1;
    sfetch_setup(&sokol_fetch); 

    // #################### Sokol Debug ####################
    #ifdef DEBUG_ON
        sg_imgui_init(&m_sg_imgui);
    #endif

    // #################### Sokol ImGui Renderer ####################
    simgui_desc_t simgui_desc {};
        simgui_desc.sample_count =     sapp_sample_count();
        simgui_desc.no_default_font =  true;
        simgui_desc.dpi_scale =        sapp_dpi_scale();
    simgui_setup(&simgui_desc);

    // ***** Set some initial ImGui styling, framed / rounded widgets
    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding =       4.0f;
    style.FrameBorderSize =     1.0f;
    style.WindowBorderSize =    1.0f;

    // Configure ImGui with default embedded dear imgui font
    auto& io = ImGui::GetIO();
    ImFontConfig fontCfg {};
        fontCfg.FontDataOwnedByAtlas = false;
        fontCfg.OversampleH = 2;
        fontCfg.OversampleV = 2;
        fontCfg.RasterizerMultiply = 1.5f;
    io.Fonts->AddFontFromMemoryTTF(dump_font, sizeof(dump_font), 16.0f, &fontCfg);

    // Create font texture for the custom font
    unsigned char* font_pixels;
    int font_width, font_height;
    io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
    sg_image_desc img_desc { };
        img_desc.width = font_width;
        img_desc.height = font_height;
        img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
        img_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
        img_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
        img_desc.min_filter = SG_FILTER_LINEAR;
        img_desc.mag_filter = SG_FILTER_LINEAR;
        img_desc.data.subimage[0][0].ptr = font_pixels;
        img_desc.data.subimage[0][0].size = static_cast<size_t>(font_width * font_height * 4);
    io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
  
    //####################################################################################

    // ***** Pass action for clearing the framebuffer to some color
    m_state.pass_action.colors[0].action = SG_ACTION_CLEAR;
    m_state.pass_action.colors[0].value = { m_bg_color.redf(), m_bg_color.greenf(), m_bg_color.bluef(), m_bg_color.alphaf() };

    // ***** Pipeline State Object, sets 3D device parameters
    sg_pipeline_desc (sokol_pipleine) { };
        sokol_pipleine.shader = sg_make_shader(extrude3D_shader_desc(sg_query_backend()));
        sokol_pipleine.layout.attrs[ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.layout.attrs[ATTR_vs_norm].format = SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.layout.attrs[ATTR_vs_texcoord0].format = SG_VERTEXFORMAT_FLOAT2; //SG_VERTEXFORMAT_SHORT2N;
        sokol_pipleine.layout.attrs[ATTR_vs_bary].format = SG_VERTEXFORMAT_FLOAT3;
        //sokol_pipleine.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
        //sokol_pipleine.index_type = SG_INDEXTYPE_NONE;
        sokol_pipleine.index_type = SG_INDEXTYPE_UINT16;
        //sokol_pipleine.cull_mode = SG_CULLMODE_NONE; 
        sokol_pipleine.cull_mode = SG_CULLMODE_FRONT;
        sokol_pipleine.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
        sokol_pipleine.depth.write_enabled = true;
        sokol_pipleine.label = "extrude-pipeline";
        sokol_pipleine.colors[0].blend = sokol_blend_alpha;
    m_state.pip = sg_make_pipeline(&sokol_pipleine);




    // ***** Font Setup, make sure the fontstash atlas size is pow-2 (all gpu textures should be, especially for webgl)
    m_state.dpi_scale = sapp_dpi_scale();
    const int atlas_size = Dr::RoundPowerOf2(512.0f * m_state.dpi_scale);
    m_state.fons = sfons_create(atlas_size, atlas_size, FONS_ZERO_TOPLEFT);
    m_state.font_normal = FONS_INVALID;       


    // #################### Virtual onCreate() ####################
    this->onCreate();
}

//####################################################################################
//##    Sokol App Events - frame (update)
//####################################################################################
void DrApp::frame(void) {
    // #################### Sokol Fetch ####################
    // Pump the sokol-fetch message queues, and invoke response callbacks
    sfetch_dowork();

    // #################### Begin Renderer ####################
    sg_begin_default_pass(&m_state.pass_action, sapp_width(), sapp_height());

    // ********** Render Here
    // Ex:
    //sg_apply_pipeline(state.pip);
    //sg_apply_bindings(&state.bind);
    //sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    //sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    //sg_draw(0, mesh->indices.size(), 1);
    
    // #################### ImGui Rendering ####################
    int width = sapp_width();
    int height = sapp_height();
    simgui_new_frame(width, height, 1.0/60.0);

    // Demo Window
    ImGui::SetNextWindowPos(ImVec2(40, 40), ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow();

    // Debug Sokol
    #ifdef DEBUG_ON
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("sokol-gfx")) {
                ImGui::MenuItem("Capabilities", 0, &sg_imgui.caps.open);
                ImGui::MenuItem("Buffers", 0, &sg_imgui.buffers.open);
                ImGui::MenuItem("Images", 0, &sg_imgui.images.open);
                ImGui::MenuItem("Shaders", 0, &sg_imgui.shaders.open);
                ImGui::MenuItem("Pipelines", 0, &sg_imgui.pipelines.open);
                ImGui::MenuItem("Passes", 0, &sg_imgui.passes.open);
                ImGui::MenuItem("Calls", 0, &sg_imgui.capture.open);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        sg_imgui_draw(&sg_imgui);
    #endif
    
    // Render ImGui
    simgui_render();
        
    // #################### End Renderer ####################
    sg_end_pass();
    sg_commit();

    // #################### Virtual onUpdate() ####################
    this->onUpdate();
}


//####################################################################################
//##    Sokol App Events - event (input, windowing, etc)
//####################################################################################
void DrApp::event(const sapp_event* e) {
    // Store event
    m_state.items[e->type].event = *e;

    // Pass event to ImGui
    simgui_handle_event(e);

    // #################### Virtual onEvent() ####################
    this->onEvent(e);
}


//####################################################################################
//##    Sokol App Events - cleanup (shutdown)
//####################################################################################
void DrApp::cleanup(void) {
    // #################### Virtual onDestroy() ####################
    this->onDestroy();

    // #################### Shut Down Sokol ####################
    sfetch_shutdown();
    sfons_destroy(m_state.fons);
    sgl_shutdown();
    simgui_shutdown();
    #ifdef DEBUG_ON
        sg_imgui_discard(&m_sg_imgui);
    #endif
    sg_shutdown();
}

