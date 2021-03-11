//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "core/imaging/Bitmap.h"
#include "core/imaging/Color.h"
#include "core/imaging/Filter.h"
#include "core/Math.h"
#include "core/Strings.h"
#include "engine/ecs/Coordinator.h"
#include "engine/scene3d/Mesh.h"
#include "App.h"
#include "Image.h"
#include "RenderContext.h"


// ##### Embed Files
#include "assets/embed/fonts/font_aileron_regular.h"


//####################################################################################
//##    Definition of Global Variables, Declared in App.h
//####################################################################################
DrApp*              g_app =         nullptr;                                        // App singleton, assigned in App:App()


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
    g_app = this;

    // Initialize Entity Component System
    m_coordinator = new DrCoordinator();        
    m_coordinator->Init();

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
    delete m_context;
    delete m_coordinator;
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
    // #################### Sokol App ####################
    m_dpi_scale = sapp_dpi_scale();

    // #################### Sokol Gfx ####################
    sg_desc sokol_gfx { };
        sokol_gfx.context = sapp_sgcontext();                   // Call sokol_glue function to auto obtain values from sokol_app      
    sg_setup(&sokol_gfx);

    // #################### Sokol Gl ####################
    sgl_desc_t sokol_gl { };
    sgl_setup(&sokol_gl);

    // #################### Sokol Time ####################
    stm_setup();
    m_time_start = stm_now();

    // #################### Sokol Fetch ####################
    // Used for loading files with the minimal "resource limits"
    sfetch_desc_t sokol_fetch { };
        sokol_fetch.max_requests = 1024;
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
           
        // Merge in icons from Font Awesome
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig icons_config { }; 
            icons_config.MergeMode = true; 
            icons_config.PixelSnapH = true;
        imgui_io.Fonts->AddFontFromFileTTF((m_app_directory + FONT_ICON_FILE_NAME_FAR).c_str(), 16.0f, &icons_config, icons_ranges);
        imgui_io.Fonts->AddFontFromFileTTF((m_app_directory + FONT_ICON_FILE_NAME_FAS).c_str(), 16.0f, &icons_config, icons_ranges);

        // Create font texture for the custom font
        unsigned char* font_pixels;
        int font_width, font_height;
        imgui_io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
        sg_image_desc img_desc { };
            img_desc.width =    font_width;
            img_desc.height =   font_height;
            img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
            img_desc.wrap_u =       SG_WRAP_CLAMP_TO_EDGE;
            img_desc.wrap_v =       SG_WRAP_CLAMP_TO_EDGE;
            img_desc.min_filter =   SG_FILTER_LINEAR;
            img_desc.mag_filter =   SG_FILTER_LINEAR;
            img_desc.data.subimage[0][0].ptr = font_pixels;
            img_desc.data.subimage[0][0].size = static_cast<size_t>(font_width * font_height * 4);
        imgui_io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
    #endif
  
    //####################################################################################
    //##    Set Up Render Context
    //##        Handles initial pipeline / bindings
    //####################################################################################
    m_context = new DrRenderContext(m_bg_color);
    
    //####################################################################################    
    //##    Load Images
    //##        NOTE: About loading images with Emscripten, when running html on local machine, must disable CORS in broswer.
    //##              On Safari, with 'Develop' menu enabled select "Disable Cross-Origin Restrictions"
    std::string image_file = m_app_directory + "assets/images/blob.png";
    //image_file = "http://github.com/stevinz/extrude/blob/master/assets/blob.png?raw=true";

    // Initiate Fetch
    loadImage(image_file);

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


//####################################################################################
//####################################################################################
//####################################################################################
//##    Image Stuff
//####################################################################################
//####################################################################################
//####################################################################################
// Start a sokol fetch of desired image
void DrApp::loadImage(std::string filename) {
    sfetch_request_t (sokol_fetch_image) {
        .path = filename.c_str(),
        .buffer_ptr = m_file_buffer,
        .buffer_size = sizeof(m_file_buffer),
        .callback = +[](const sfetch_response_t* response) {
            if (response->fetched) {
                // File data has been fetched, since we provided a big-enough buffer we can be sure that all data has been loaded here
                g_app->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
            } else if (response->finished) {
                // If loading the file failed, set clear color to signal reason
                if (response->failed) { /*response->error_code*/ }
            }
        },       
    };
    sfetch_send(&sokol_fetch_image);
}


//####################################################################################
//##    Load Image
//####################################################################################
void DrApp::initImage(stbi_uc* buffer_ptr, int fetched_size) {
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
        
        // To store an image onto the gpu:
        long image_id = sg_make_image(&sokol_image).id;

        // If we already have an image in the state buffer, uninit before initializing new image
        if (m_initialized_image == true) { sg_uninit_image(m_context->bindings.fs_images[SLOT_tex]); }

        // Initialize new image into state buffer
        sg_init_image(m_context->bindings.fs_images[SLOT_tex], &sokol_image);
        m_initialized_image = true;
        stbi_image_free(pixels);
    }
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
        sg_destroy_buffer(m_context->bindings.vertex_buffers[0]);
        m_context->bindings.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

        // ***** Index Buffer
        unsigned int total_indices = m_mesh->indices.size();
        uint16_t indices[total_indices];
        for (size_t i = 0; i < total_indices; i++) indices[i] = m_mesh->indices[i];
        sg_buffer_desc (sokol_buffer_index) {
            .type = SG_BUFFERTYPE_INDEXBUFFER,
            .data = SG_RANGE(indices),
            .label = "temp-indices"
        };
        sg_destroy_buffer(m_context->bindings.index_buffer);
        m_context->bindings.index_buffer = sg_make_buffer(&(sokol_buffer_index));

        // ***** Reset rotation
        if (reset_position) {
            m_total_rotation.set(0.f, 0.f);
            m_add_rotation.set(25.f, 25.f);
            m_model = DrMatrix::identityMatrix();
        }
    }
}


