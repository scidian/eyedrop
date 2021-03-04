//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
// ***** Also includes Sokol
#include "App.h"

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
#include "core/Math.h"
#include "core/Strings.h"
#include "shaders/basic_shader.glsl.h"


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


// static void font_normal_loaded(const sfetch_response_t* response) {
//     if (response->fetched) {
//         state.font_normal = fonsAddFontMem(state.fons, "sans", (unsigned char*)response->buffer_ptr, (int)response->fetched_size,  false);
//     } 
// }

//####################################################################################
//##    Sokol File Scope Stuff
//####################################################################################
// Local reference to DrApp Singleton for Sokol App
DrApp   l_app;          

// ***** C callback wrappers for using member methods as callbacks functions
void initWrapper(void)                      { l_app.init(); }
void frameWrapper(void)                     { l_app.frame(); }
void eventWrapper(const sapp_event *event)  { l_app.event(event); }
void cleanupWrapper(void)                   { l_app.cleanup(); }


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrApp::DrApp(std::string title, DrColor bg_color, int width, int height) {
    m_app_name = title;
    m_bg_color = bg_color;

    l_app = *this;

    m_sokol_app.window_title =          m_app_name.c_str();
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
//##    Getters / setters
//####################################################################################
// Sets application name, updates title bar
void DrApp::setAppName(std::string name) { 
    m_app_name = name; 
    sapp_set_window_title(m_app_name.c_str());
}


//####################################################################################
//##    Sokol App Events - Init
//####################################################################################
// Initializes all sokol libraries
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
    const int atlas_size = RoundPowerOf2(512.0f * m_state.dpi_scale);
    m_state.fons = sfons_create(atlas_size, atlas_size, FONS_ZERO_TOPLEFT);
    m_state.font_normal = FONS_INVALID;       

    // ***** Start loading the PNG File
    //  We don't need the returned handle since we can also get that inside the fetch-callback from the response
    //  structure. NOTE: we're not using the user_data member, since all required state is in a global variable anyway
    char* path = NULL;
    int length, dirname_length;
    std::string image_file = "", font_file = "";

    #ifndef DROP_TARGET_HTML5
        length = wai_getExecutablePath(NULL, 0, &dirname_length);
        if (length > 0) {
            path = (char*)malloc(length + 1);
            wai_getExecutablePath(path, length, &dirname_length);
            //path[length] = '\0';
            //printf("executable path: %s\n", path);
            path[dirname_length] = '\0';
            //printf("  dirname: %s\n", path);
            //printf("  basename: %s\n", path + dirname_length + 1);
            std::string base = path;
            image_file = base + "/assets/blob.png";
            font_file  = base + "/assets/aileron-regular.otf";
            free(path);
        }
    #else        
        // ********** NOTE: About loading images with Emscripten **********
        //  When running html on local machine, must disable CORS in broswer
        //  On Safari, with 'Develop' menu enabled select "Disable Cross-Origin Restrictions"
        //image_file = "http://github.com/stevinz/extrude/blob/master/assets/shapes.png?raw=true";
        image_file = "assets/shapes.png";
        font_file  = "assets/aileron-regular.otf";
    #endif

    // Load font in background
    #ifndef DROP_TARGET_HTML5
        sfetch_request_t (sokol_fetch_font) {
            .path = font_file.c_str(),
            .buffer_ptr = m_state.font_normal_data,
            .buffer_size = sizeof(m_state.font_normal_data),
            .user_void_ptr = this,
            .callback = +[](const sfetch_response_t* response) {
                DrApp *app = (DrApp*)(response->user_void_ptr);
                if (response->fetched && app) {
                    app->m_state.font_normal = fonsAddFontMem(app->m_state.fons, "sans", (unsigned char*)response->buffer_ptr, (int)response->fetched_size, false);
                } 
            },       
        };
        sfetch_send(&sokol_fetch_font);
    #endif


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

    // #################### Virtual onUpdate() - User Rendering ####################
    this->onUpdate();
    

    // #################### Fontstash Text Rendering ####################
    fonsClearState(m_state.fons);    
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, sapp_widthf(), sapp_heightf(), 0.0f, -1.0f, +1.0f);

    const float dpis = m_state.dpi_scale;
    FONScontext* fs = m_state.fons;

    if (m_state.font_normal != FONS_INVALID) {
        fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
        fonsSetFont(fs, m_state.font_normal);
        fonsSetSize(fs, 18.0f * dpis);
        fonsSetColor(fs, sfons_rgba(255, 255, 255, 255));;
        fonsSetBlur(fs, 0);
        fonsSetSpacing(fs, 0.0f);
        fonsDrawText(fs, 10 * dpis, 40 * dpis, ("FPS: " + RemoveTrailingZeros(std::to_string(framesPerSecond()))).c_str(), NULL);
    }
    sfons_flush(fs);            // Flush fontstash's font atlas to sokol-gfx texture
    sgl_draw();


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
                ImGui::MenuItem("Capabilities", 0, &m_sg_imgui.caps.open);
                ImGui::MenuItem("Buffers", 0, &m_sg_imgui.buffers.open);
                ImGui::MenuItem("Images", 0, &m_sg_imgui.images.open);
                ImGui::MenuItem("Shaders", 0, &m_sg_imgui.shaders.open);
                ImGui::MenuItem("Pipelines", 0, &m_sg_imgui.pipelines.open);
                ImGui::MenuItem("Passes", 0, &m_sg_imgui.passes.open);
                ImGui::MenuItem("Calls", 0, &m_sg_imgui.capture.open);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        sg_imgui_draw(&m_sg_imgui);
    #endif
    
    // Render ImGui
    simgui_render();

    // #################### End Renderer ####################
    sg_end_pass();
    sg_commit();

    // #################### FPS ####################
    static uint64_t last_time =     0;
    static double   lap_time =      0.0;
    static int      frame_count =   0;
    lap_time += stm_sec(stm_laptime(&last_time));
    frame_count++;
    if (lap_time >= 1.0) {
        m_frames_per_second = frame_count; //1.0 / stm_sec(stm_laptime(&last_time));
        frame_count = 0;
        lap_time =  0.0;
    }
}


//####################################################################################
//##    Sokol App Events - event (input, windowing, etc)
//####################################################################################
void DrApp::event(const sapp_event *event) {
    // Store event
    m_state.items[event->type].event = *event;

    // Pass event to ImGui
    simgui_handle_event(event);

    // #################### Virtual onEvent() ####################
    this->onEvent(event);
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

