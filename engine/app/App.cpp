//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "engine/app/core/Math.h"
#include "engine/app/core/Reflect.h"
#include "engine/app/core/Strings.h"
#include "engine/app/image/Bitmap.h"
#include "engine/app/image/Color.h"
#include "engine/app/image/Filter.h"
#include "engine/app/image/Image.h"
#include "engine/app/imgui/ImUtilities.h"
#include "engine/app/resources/ImageManager.h"
#include "engine/ecs/Coordinator.h"
#include "engine/scene3d/Mesh.h"
#include "App.h"
#include "RenderContext.h"

// ##### Embed Files
#include "assets/embed/fonts/font_aileron_regular.h"


//####################################################################################
//##    Global Variables Definitions
//####################################################################################
DrApp*              g_app_pointer   { nullptr };                                    // App singleton            Declared in App.h       Assigned in App:App()
DrApp*              App()           { return g_app_pointer; }                       // Returns App singleton 


//####################################################################################
//##    Callbacks
//####################################################################################
// C callback wrappers for using member methods as callbacks functions
std::function<void()>                                   initCallback;     
std::function<void()>                                   frameCallback;     
std::function<void(const sapp_event* event)>            eventCallback;      
std::function<void()>                                   cleanupCallback;    
extern "C" void initWrapper()                           { initCallback(); }
extern "C" void frameWrapper()                          { frameCallback(); }
extern "C" void eventWrapper(const sapp_event* event)   { eventCallback(event); }
extern "C" void cleanupWrapper()                        { cleanupCallback(); }


//####################################################################################
//##    Constructor / Destructor 
//####################################################################################
DrApp::DrApp(std::string title, DrColor bg_color, int width, int height) {
    // Assign global pointer to the current App
    g_app_pointer = this;

    // Set locals
    m_app_name = title;
    m_bg_color = bg_color;

    // Initialize Sokol App
    initCallback =      std::bind(&DrApp::init,     this);
    frameCallback =     std::bind(&DrApp::frame,    this);
    eventCallback =     std::bind(&DrApp::event,    this, std::placeholders::_1);
    cleanupCallback =   std::bind(&DrApp::cleanup,  this);
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

// Destructor
DrApp::~DrApp() {
    delete m_image_manager;
    delete m_context;

    // Mac Menu Bar Cleanup
    ImUtilities::MenuShutDown();
}

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
    // #################### Mac Menu Bar ####################
    #if defined(DROP_TARGET_OSX) && defined(DROP_TARGET_OSX_MENUS)
        ImUtilities::TargetOS(UTILITIES_TARGET_MACOS);
    #endif
    ImUtilities::MenuInitialize(m_app_name.c_str());
        
    // #################### Sokol App ####################
    setAppName(m_app_name);                                     // Set initial Window Title
    m_dpi_scale = sapp_dpi_scale();

    // #################### Sokol Gfx ####################
    sg_desc sokol_gfx { };
        sokol_gfx.context = sapp_sgcontext();                   // Call sokol_glue function to auto obtain values from sokol_app      
    sg_setup(&sokol_gfx);
    m_sg_features = sg_query_features();
    m_sg_limits = sg_query_limits();
    assert(sg_isvalid() && "Could not initialize Sokol Gfx!!");
    assert(m_sg_features.instancing && "Instancing not supported!!");

    // #################### Sokol Gl ####################
    sgl_desc_t sokol_gl { };
    sgl_setup(&sokol_gl);

    // #################### Sokol Time ####################
    stm_setup();
    m_time_start = stm_now();

    // #################### Sokol Fetch ####################
    // Used for loading files with the minimal "resource limits"
    sfetch_desc_t sokol_fetch { };
        sokol_fetch.max_requests = 128;
        sokol_fetch.num_channels = 4;
        sokol_fetch.num_lanes =    2;
    sfetch_setup(&sokol_fetch); 

    // #################### Sokol Debug ####################
    #if defined (ENABLE_DEBUG)
        sg_imgui_init(&m_sg_imgui);
    #endif

    // #################### Store local app directory ####################
    #ifndef DROP_TARGET_HTML5
        char* path = NULL;
        int dirname_length = 0;
        int length = wai_getExecutablePath(NULL, 0, &dirname_length);
        if (length > 0) {
            path = (char*)malloc(length + 1);
            wai_getExecutablePath(path, length, &dirname_length);
            path[dirname_length] = '\0';
            m_app_directory = path;
            m_app_directory += "/";
            free(path);
        }
    #endif

    // #################### Fontstash  ####################     
    // Ensure atlas size is pow-2 (all gpu textures should be, especially webgl)
    const int atlas_size = RoundPowerOf2(256.0f * m_dpi_scale);           
    m_fontstash =   sfons_create(atlas_size, atlas_size, FONS_ZERO_TOPLEFT);
    m_font_normal = fonsAddFontMem(m_fontstash, "sans", aileron, sizeof(aileron), false);

    //####################################################################################
    //##    Sokol ImGui Renderer
    //####################################################################################
    #if defined (ENABLE_IMGUI)
        simgui_desc_t simgui_desc { };
            simgui_desc.sample_count =     sapp_sample_count();
            simgui_desc.no_default_font =  true;
            simgui_desc.dpi_scale =        m_dpi_scale;
        simgui_setup(&simgui_desc);

        // Set some initial ImGui styling, framed / rounded widgets
        ImGuiStyle& style = ImGui::GetStyle();
            style.Alpha =               0.98f;
            style.FrameRounding =       6.f;
            style.FrameBorderSize =     1.f;
            style.FramePadding =        ImVec2(4.f, 4.f);
            style.GrabRounding =        6.f;
            style.IndentSpacing =       16.0f;
            style.ItemInnerSpacing =    ImVec2(8.f, 4.f);
            style.ItemSpacing =         ImVec2(8.f, 4.f);
            style.PopupRounding =       0.f;
            style.TabBorderSize =       1.0f;
            style.TabRounding =         6.f;
            style.WindowBorderSize =    1.f;
            style.WindowRounding =      0.f;
            style.WindowPadding =       ImVec2(3.f, 3.f);
            style.WindowTitleAlign =    ImVec2(0.5f, 0.5f);
            style.WindowMenuButtonPosition = ImGuiDir_None; // (default: ImGuiDir_Left)

        // Configure ImGui, enable docking 
        auto &imgui_io = ImGui::GetIO();
            imgui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        
        // Attach default embedded font
        ImFontConfig fontCfg { };
            fontCfg.FontDataOwnedByAtlas = false;
            fontCfg.OversampleH = 2;
            fontCfg.OversampleV = 2;
            fontCfg.RasterizerMultiply = 1.5f;
        imgui_io.Fonts->AddFontFromMemoryTTF(aileron, sizeof(aileron), 16.0f, &fontCfg);
           
        // If including font icons, merge in icons from Font Awesome
        /*
            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            ImFontConfig icons_config { }; 
                icons_config.MergeMode = true; 
                icons_config.PixelSnapH = true;
            imgui_io.Fonts->AddFontFromFileTTF((m_app_directory + FONT_ICON_FILE_NAME_FAR).c_str(), 16.0f, &icons_config, icons_ranges);
            imgui_io.Fonts->AddFontFromFileTTF((m_app_directory + FONT_ICON_FILE_NAME_FAS).c_str(), 16.0f, &icons_config, icons_ranges);
        */

        // Create font texture for the custom font
        unsigned char* font_pixels;
        int font_width, font_height;
        imgui_io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
        sg_image_desc image_desc { };
            DrImageManager::initializeSgImageDesc(font_width, font_height, image_desc);
            image_desc.data.subimage[0][0].ptr = font_pixels;
            image_desc.data.subimage[0][0].size = static_cast<size_t>(font_width * font_height * 4);
        imgui_io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&image_desc).id;
    #endif

  
    //####################################################################################
    //##    App Singletons
    //####################################################################################
    m_image_manager = new DrImageManager();                                             // Image Manager: Helps with image loading / fetching, atlas creation
    m_context = new DrRenderContext(m_bg_color);                                        // Render Context: Handles initial pipeline / bindings
    

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

    // Check for images to load
    if (m_image_manager) m_image_manager->processFetchStack();

    // #################### Begin Renderer ####################
    sg_begin_default_pass(&m_context->pass_action, sapp_width(), sapp_height());
    
    // ********** Render Here, Scene 2D, Scene 3D, Etc...
    // Ex:
    //sg_apply_pipeline(m_state.pip);
    //sg_apply_bindings(&m_state.bind);
    //sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    //sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    //sg_draw(0, mesh->indices.size(), 1);
    
    // #################### Virtual onUpdate() - User Rendering ####################
    this->onUpdateScene();
    
    // #################### ImGui Rendering ####################
    #if defined (ENABLE_IMGUI)
        // Start ImGui Frame
        int width = sapp_width();
        int height = sapp_height();
        simgui_new_frame(width, height, 1.0/60.0);

        // #################### Virtual onUpdate() - User Rendering ####################
        this->onUpdateGUI();
        // ####################

        // Debug Sokol
        #if defined (ENABLE_DEBUG)
            if (ImUtilities::BeginMainMenuBar()) {
                if (ImUtilities::BeginMenu("sokol-gfx")) {
                    ImUtilities::MenuItem("Capabilities",   0, &m_sg_imgui.caps.open);
                    ImUtilities::MenuItem("Buffers",        0, &m_sg_imgui.buffers.open);
                    ImUtilities::MenuItem("Images",         0, &m_sg_imgui.images.open);
                    ImUtilities::MenuItem("Shaders",        0, &m_sg_imgui.shaders.open);
                    ImUtilities::MenuItem("Pipelines",      0, &m_sg_imgui.pipelines.open);
                    ImUtilities::MenuItem("Passes",         0, &m_sg_imgui.passes.open);
                    ImUtilities::MenuItem("Calls",          0, &m_sg_imgui.capture.open);
                    ImUtilities::EndMenu();
                }
                ImUtilities::EndMainMenuBar();
            }
            sg_imgui_draw(&m_sg_imgui);
        #endif
        
        // Render ImGui
        if (m_first_frame == false) {
            simgui_render();
        } else {
            ImGui::EndFrame();
        }
    #endif

    // #################### Fontstash Text Rendering ####################
    fonsClearState(m_fontstash);    
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, sapp_widthf(), sapp_heightf(), 0.0f, -1.0f, +1.0f);
    if (m_font_normal != FONS_INVALID) {
        fonsSetAlign(m_fontstash,     FONS_ALIGN_LEFT | FONS_ALIGN_TOP); //FONS_ALIGN_BASELINE);
        fonsSetFont(m_fontstash,      m_font_normal);
        fonsSetSize(m_fontstash,      16.0f * m_dpi_scale);
        fonsSetColor(m_fontstash,     sfons_rgba(255, 255, 255, 255));;
        fonsSetBlur(m_fontstash,      0);
        fonsSetSpacing(m_fontstash,   0.0f);
        fonsDrawText(m_fontstash, sapp_width() - (60 * m_dpi_scale), (4 * m_dpi_scale), ("FPS: " + RemoveTrailingZeros(std::to_string(framesPerSecond()))).c_str(), NULL);
    }
    sfons_flush(m_fontstash);                     // Flush fontstash's font atlas to sokol-gfx texture
    sgl_draw();

    // #################### End Renderer ####################
    sg_end_pass();
    sg_commit();

    // #################### Delta Time / FPS ####################
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

    // First frame has been processed
    // This variable allows for some preprocessing before rendering for the first time (like ImGui colors, sizes)
    m_first_frame = false;
}


//####################################################################################
//##    Sokol App Events - event (input, windowing, etc)
//####################################################################################
void DrApp::event(const sapp_event* event) {
    // Pass event to ImGui
    #if defined (ENABLE_IMGUI)
        simgui_handle_event(event);
    #endif

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
    sfons_destroy(m_fontstash);
    sgl_shutdown();
    #if defined (ENABLE_IMGUI)
        simgui_shutdown();
        #if defined (ENABLE_DEBUG)
            sg_imgui_discard(&m_sg_imgui);
        #endif
    #endif
    sg_shutdown();
}



