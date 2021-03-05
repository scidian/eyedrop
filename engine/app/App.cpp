//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
// ##### Also includes Sokol
#include "App.h"

// ##### Fonts
#include "3rd_party/fontstash.h"
#include "3rd_party/sokol/sokol_fontstash.h"

// ##### ImGui
#include "3rd_party/imgui/imgui.h"
#define SOKOL_IMGUI_IMPL
#include "3rd_party/sokol/sokol_imgui.h"

// ##### Debug Menu
#ifdef DEBUG_ON
    #define SOKOL_GFX_IMGUI_IMPL
    #include "3rd_party/sokol/sokol_gfx_imgui.h"
#endif

// ##### Html5 / File Handling
#ifndef DROP_TARGET_HTML5
    #include "3rd_party/wai/whereami.c"
#else
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#endif

// ##### Engine
#include "core/imaging/Bitmap.h"
#include "core/imaging/Color.h"
#include "core/imaging/Filter.h"
#include "core/imaging/Image.h"
#include "core/Math.h"
#include "core/Strings.h"
#include "../scene3d/Mesh.h"
#include "../scene3d/shaders/BasicShader.glsl.h"


// ##### Embed Files
#include "assets/embed/fonts/font_aileron_regular.h"


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
DrApp* l_app = nullptr;          

// ***** C callback wrappers for using member methods as callbacks functions
void initWrapper(void)                      { l_app->init(); }
void frameWrapper(void)                     { l_app->frame(); }
void eventWrapper(const sapp_event *event)  { l_app->event(event); }
void cleanupWrapper(void)                   { l_app->cleanup(); }


//####################################################################################
//##    Callback: File Loading
//####################################################################################
/* The fetch-callback is called by sokol_fetch.h when the data is loaded,
   or when an error has occurred. */
static void image_loaded(const sfetch_response_t* response) {
    if (response->fetched) {
        // File data has been fetched
        //  Since we provided a big-enough buffer we can be sure that all data has been loaded here
        l_app->loadImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
    
    } else if (response->finished) {
        // If loading the file failed, set clear color to signal reason
        if (response->failed) {
            sg_pass_action (pass_action0) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 1.0f, 1.0f, 1.0f } } };        // white
            sg_pass_action (pass_action1) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 0.0f, 0.0f, 1.0f } } };        // red
            sg_pass_action (pass_action2) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.0f, 1.0f, 0.0f, 1.0f } } };        // green
            sg_pass_action (pass_action3) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.0f, 0.0f, 1.0f, 1.0f } } };        // blue 
            sg_pass_action (pass_action4) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 1.0f, 0.0f, 1.0f } } };        // yellow
            sg_pass_action (pass_action5) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.0f, 1.0f, 1.0f, 1.0f } } };        // cyan
            sg_pass_action (pass_action6) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 0.0f, 1.0f, 1.0f } } };        // magenta
            sg_pass_action (pass_action7) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.5f, 0.5f, 0.5f, 1.0f } } };        // black
    
            switch (response->error_code) {
                case SFETCH_ERROR_NO_ERROR:             l_app->m_state.pass_action = (pass_action0);     break;
                case SFETCH_ERROR_FILE_NOT_FOUND:       l_app->m_state.pass_action = (pass_action1);     break;
                case SFETCH_ERROR_NO_BUFFER:            l_app->m_state.pass_action = (pass_action2);     break;
                case SFETCH_ERROR_BUFFER_TOO_SMALL:     l_app->m_state.pass_action = (pass_action3);     break;
                case SFETCH_ERROR_UNEXPECTED_EOF:       l_app->m_state.pass_action = (pass_action4);     break;
                case SFETCH_ERROR_CANCELLED:            l_app->m_state.pass_action = (pass_action5);     break;
                case SFETCH_ERROR_INVALID_HTTP_STATUS:  l_app->m_state.pass_action = (pass_action6);     break;
                default:                                l_app->m_state.pass_action = (pass_action7);
            }            
        }
    }
}


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrApp::DrApp(std::string title, DrColor bg_color, int width, int height) {
    m_app_name = title;
    m_bg_color = bg_color;

    l_app = this;

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
//##    Create 3D extrusion
//####################################################################################
void DrApp::calculateMesh(bool reset_position) {
    //##    Level of Detail:
    //##        0.075 = Detailed
    //##        0.250 = Nice
    //##        1.000 = Low poly
    //##       10.000 = Really low poly
    float level_of_detail = 0.6f;
    switch (m_mesh_quality) {
        case 0: level_of_detail = 19.200f;  break;
        case 1: level_of_detail =  9.600f;  break;
        case 2: level_of_detail =  4.800f;  break;
        case 3: level_of_detail =  2.400f;  break;
        case 4: level_of_detail =  1.200f;  break;
        case 5: level_of_detail =  0.600f;  break;
        case 6: level_of_detail =  0.300f;  break;
        case 7: level_of_detail =  0.150f;  break;
        case 8: level_of_detail =  0.075f;  break;
    }

    // ***** Initialize Mesh
    m_image->outlinePoints(level_of_detail);
    m_mesh = std::make_shared<DrMesh>();
    m_mesh->image_size = Max(m_image->getBitmap().width, m_image->getBitmap().height);      
    m_mesh->wireframe = m_wireframe;
    m_mesh->initializeExtrudedImage(m_image.get(), m_mesh_quality);
    //mesh->initializeTextureQuad();
    //mesh->initializeTextureCube();
    //mesh->initializeTextureCone();
            
    // ***** Copy vertex data and set into state buffer
    if (m_mesh->vertexCount() > 0) {
        // ***** Vertex Buffer
        unsigned int total_vertices = m_mesh->vertices.size();

        Vertex vertices[total_vertices];
        for (size_t i = 0; i < total_vertices; i++) vertices[i] = m_mesh->vertices[i];
        sg_buffer_desc (sokol_buffer_vertex) {
            .data = SG_RANGE(vertices),
            .label = "extruded-vertices"
        };
        sg_destroy_buffer(m_state.bind.vertex_buffers[0]);
        m_state.bind.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

        // ***** Index Buffer
        unsigned int total_indices = m_mesh->indices.size();
        uint16_t indices[total_indices];
        for (size_t i = 0; i < total_indices; i++) indices[i] = m_mesh->indices[i];
        sg_buffer_desc (sokol_buffer_index) {
            .type = SG_BUFFERTYPE_INDEXBUFFER,
            .data = SG_RANGE(indices),
            .label = "temp-indices"
        };
        sg_destroy_buffer(m_state.bind.index_buffer);
        m_state.bind.index_buffer = sg_make_buffer(&(sokol_buffer_index));

        // ***** Reset rotation
        if (reset_position) {
            m_total_rotation.set(0.f, 0.f);
            m_add_rotation.set(25.f, 25.f);
            m_model = DrMatrix::identityMatrix();
        }
    }
}


//####################################################################################
//##    Load Image
//####################################################################################
void DrApp::loadImage(stbi_uc* buffer_ptr, int fetched_size) {
    int png_width, png_height, num_channels;
    const int desired_channels = 4;
    stbi_uc* pixels = stbi_load_from_memory(buffer_ptr, fetched_size, &png_width, &png_height, &num_channels, desired_channels);

    // Stb Load Succeeded
    if (pixels) {
        // ********** Copy data into our custom bitmap class, create image and trace outline
        DrBitmap bitmap(pixels, static_cast<int>(png_width * png_height * 4), false, png_width, png_height);
        //bitmap = Dr::ApplySinglePixelFilter(DROP_IMAGE_FILTER_HUE, bitmap, Dr::RandomInt(-100, 100));
        m_image = std::make_shared<DrImage>("shapes", bitmap, 0.25f);

        calculateMesh(true);        

        // ********** Initialze the sokol-gfx texture
        sg_image_desc (sokol_image) {
            .width =        m_image->getBitmap().width,
            .height =       m_image->getBitmap().height,
            .pixel_format = SG_PIXELFORMAT_RGBA8,
            .wrap_u =       SG_WRAP_MIRRORED_REPEAT,
            .wrap_v =       SG_WRAP_MIRRORED_REPEAT,
            .min_filter =   SG_FILTER_LINEAR,
            .mag_filter =   SG_FILTER_LINEAR,
            .data.subimage[0][0] = {
                .ptr =  &(m_image->getBitmap().data[0]),
                .size = (size_t)m_image->getBitmap().size(),
            }
        };
        
        // If we already have an image in the state buffer, uninit before initializing new image
        if (m_initialized_image == true) { sg_uninit_image(m_state.bind.fs_images[SLOT_tex]); }

        // Initialize new image into state buffer
        sg_init_image(m_state.bind.fs_images[SLOT_tex], &sokol_image);
        m_initialized_image = true;
        stbi_image_free(pixels);

        std::cout << "Done preparing image..." << std::endl;
        std::cout << "Has initialized image: " << m_initialized_image << std::endl;
    }
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
    auto &imgui_io = ImGui::GetIO();
    imgui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImFontConfig fontCfg { };
        fontCfg.FontDataOwnedByAtlas = false;
        fontCfg.OversampleH = 2;
        fontCfg.OversampleV = 2;
        fontCfg.RasterizerMultiply = 1.5f;
    imgui_io.Fonts->AddFontFromMemoryTTF(aileron, sizeof(aileron), 16.0f, &fontCfg);

    // Create font texture for the custom font
    unsigned char* font_pixels;
    int font_width, font_height;
    imgui_io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
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
    imgui_io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
  
    //####################################################################################
    //##    Set Up Pipeline
    //####################################################################################
    // ***** Pass action for clearing the framebuffer to some color
    m_state.pass_action.colors[0].action = SG_ACTION_CLEAR;
    m_state.pass_action.colors[0].value = { m_bg_color.redf(), m_bg_color.greenf(), m_bg_color.bluef(), m_bg_color.alphaf() };

    // ***** Pipeline State Object, sets 3D device parameters
    sg_pipeline_desc (sokol_pipleine) { };
        sokol_pipleine.shader = sg_make_shader(basic_shader_shader_desc(sg_query_backend()));
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

    // ***** Allocate an image handle, 
    //  but don't actually initialize the image yet, this happens later when the asynchronous file load has finished.
    //  Any draw calls containing such an "incomplete" image handle will be silently dropped.
    m_state.bind.fs_images[SLOT_tex] = sg_alloc_image();

    // ***** Starter triangle
    // Vertex buffer
    const Vertex vertices[] = {
        // pos                  normals                uvs          barycentric (wireframe)
        { -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      0,   0,      1.0f, 1.0f, 1.0f },
        {  1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      1,   0,      1.0f, 1.0f, 1.0f },
        {  1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      1,   1,      1.0f, 1.0f, 1.0f },      
        { -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      0,   1,      1.0f, 1.0f, 1.0f },      
    };
    sg_buffer_desc (sokol_buffer_vertex) {
        .data = SG_RANGE(vertices),
        .label = "temp-vertices"
    };
    m_state.bind.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

    // Index buffer
    const uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };
    sg_buffer_desc (sokol_buffer_index) {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "temp-indices"
    };
    m_state.bind.index_buffer = sg_make_buffer(&(sokol_buffer_index));

    //####################################################################################

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

    // Load inital "shapes.png" image in background
    sfetch_request_t (sokol_fetch_image) {
        .path = image_file.c_str(),
        .callback = image_loaded,
        .buffer_ptr = m_state.file_buffer,
        .buffer_size = sizeof(m_state.file_buffer)
    };
    sfetch_send(&sokol_fetch_image);

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
    
    // ********** Render Here, Scene 2D, Scene 3D, Etc...
    // Ex:
    //sg_apply_pipeline(m_state.pip);
    //sg_apply_bindings(&m_state.bind);
    //sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    //sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    //sg_draw(0, mesh->indices.size(), 1);
    
    // #################### Virtual onUpdate() - User Rendering ####################
    this->onUpdateScene();
    
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
    ImVec2 win_pos = ImGui::GetMainViewport()->Pos;
           win_pos.x += 80;
           win_pos.y += 80;
    ImGui::SetNextWindowPos(win_pos, ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow();

    // #################### Virtual onUpdate() - User Rendering ####################
    this->onUpdateGUI();
    // ####################

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
        m_frames_per_second = frame_count;      //1.0 / stm_sec(stm_laptime(&last_time));
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

