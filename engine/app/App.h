//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_APP_H
#define DR_APP_H

// ##### STL
#include <fstream>
#include <map>
#include <string>

// ##### 3rd Party
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/sokol/sokol_gl.h"
#if defined (ENABLE_DEBUG)
    #include "3rd_party/sokol/sokol_gfx_imgui.h"
#endif
#include "3rd_party/sokol/sokol_glue.h"
#include "3rd_party/sokol/sokol_time.h"
#include "3rd_party/sokol/sokol_audio.h"
#include "3rd_party/sokol/sokol_fetch.h"
#include "3rd_party/fontstash.h"
#include "3rd_party/sokol/sokol_fontstash.h"
#include "3rd_party/stb/stb_image.h"
#ifndef DROP_TARGET_HTML5
    #include "3rd_party/cereal/types/map.hpp"
    #include "3rd_party/cereal/types/memory.hpp"
    #include "3rd_party/cereal/types/string.hpp"
    #include "3rd_party/cereal/types/set.hpp"
    #include "3rd_party/cereal/archives/binary.hpp"
    #include "3rd_party/whereami.h"
#else
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#endif
#include "3rd_party/handmade_math.h"
#if defined (ENABLE_IMGUI)
    #include "3rd_party/imgui/imgui.h"
    #include "3rd_party/imgui/imgui_internal.h"
    #include "3rd_party/icons_font_awesome5.h"
    #include "3rd_party/sokol/sokol_imgui.h"
#endif

// ##### Engine
#include "core/geometry/Matrix.h"
#include "core/geometry/Vec2.h"
#include "core/imaging/Color.h"
#include "engine/data/Game.h"
#include "engine/data/Project.h"
#include "engine/data/Types.h"
#include "engine/scene3d/Mesh.h"

// Forward Declarations
class DrApp;
class DrRenderContext;

//####################################################################################
//##    Constants 
//############################
#define MAX_FILE_SIZE   (1024 * 1024)                                               // Used for filebuffers with sokol_fetch
#define INVALID_IMAGE   -1                                                          // Used to identify DrImages that have been initialized, but not loaded yet


//####################################################################################
//##    Local Structs / Defines
//############################
typedef std::map<std::string, std::shared_ptr<DrProject>>    GameMap;               // Holds data of open Game instances
typedef std::map<std::string, std::shared_ptr<DrProject>>    ProjectMap;            // Holds data of open Projects


//####################################################################################
//##    DrApp
//##        Base class of a Drop Application
//############################
class DrApp 
{
public:
    // Constructor / Destructor
    DrApp(std::string title = "Drop Creator", DrColor bg_color = DROP_COLOR_BLACK, int width = 800, int height = 600);
    ~DrApp();

    // Returns pointer to the current running App
    static DrApp* GetApp();

    // #################### VARIABLES ####################
protected:
    // Modules
    sapp_desc               m_sokol_app;                                            // Sokol_app descriptor for this Window
    DrRenderContext*        m_context               { nullptr };                    // Rendering context for this App (currently built on Sokol_Gfx)

    // Data
    GameMap                 m_game                  { };                            // Collection of open Game instances
    ProjectMap              m_projects              { };                            // Collection of open Projects
    
    // App Variables
    std::string             m_app_name              { "" };                         // Name of Application   
    std::string             m_app_directory         { "" };                         // Root OS directory of application
    DrColor                 m_bg_color              { DROP_COLOR_BLACK };           // Background color of main App
    int                     m_width                 { 800 };                        // Window width
    int                     m_height                { 600 };                        // Window height
    float                   m_dpi_scale             { 1.f };                        // Dpi scale of device we're running on

    // Fetch / Drag & Drop Buffers
    uint8_t                 m_file_buffer[MAX_FILE_SIZE];
    uint8_t                 m_file_buffer2[MAX_FILE_SIZE];

    // Fonts
    FONScontext*            m_fontstash;
    int                     m_font_normal = INVALID_IMAGE;
    uint8_t                 m_font_normal_data[MAX_FILE_SIZE];

    // ImGui, disabled by default
    #if defined (ENABLE_DEBUG)
        sg_imgui_t          m_sg_imgui;                                             // Sokol_gfx_debug keeps track of data structures used by sokol_gfx for Debug View
    #endif

    // Time Variables
    bool                    m_first_frame       { true };                           // Turns false after first frame, allows for some initialization (colors, themeing, etc)
    uint64_t                m_time_start        { 0 };                              // Sokol_time start time since App started running
    double                  m_frames_per_second { 0.0 };                            // Stores current calculated frames per second


    // ---> Temp Variables, used for demo
    std::shared_ptr<DrMesh>     m_mesh = std::make_shared<DrMesh>();
    std::shared_ptr<DrImage>    m_image = nullptr;                              
    int                         m_mesh_quality = 5;

    // Image Variables
    bool                        m_initialized_image = false;
    int                         m_before_keys = m_mesh_quality;

    // Model Rotation
    DrVec2                      m_total_rotation {  0.f,  0.f };
    DrVec2                      m_add_rotation   { 25.f, 25.f };
    hmm_mat4                    m_model =         DrMatrix::identityMatrix();
    DrVec2                      m_mouse_down =    { 0, 0 };
    float                       m_rotate_speed =  1.f;
    bool                        m_is_mouse_down = false;
    float                       m_zoom = 1.5f;
    bool                        m_wireframe = true;
    // <--- End Temp Variables


    
    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    virtual void    onCreate(void) { }
    virtual void    onUpdateScene(void) { }
    virtual void    onUpdateGUI(void) { }
    virtual void    onEvent(const sapp_event* event) { }
    virtual void    onDestroy(void) { }
    

    // #################### INTERNAL FUNCTIONS ####################
public:
    // Mesh
    void calculateMesh(bool reset_position);
    void loadImage(std::string filename);
    void initImage(stbi_uc *buffer_ptr, int fetched_size);

    // Sokol Related
    void run() { sapp_run(m_sokol_app); }

    // Linked to internal sokol callbacks
    void init(void);
    void frame(void);
    void event(const sapp_event *event);
    void cleanup(void);

    // Local Variable Functions
    std::string         appName()                                       { return m_app_name; }
    void                setAppName(std::string name);
    DrColor             backgroundColor()                               { return m_bg_color; }
    int                 getWidth() { return m_width; }
    int                 getHeight() { return m_height; }

    // Timer Functions
    double              framesPerSecond()                               { return m_frames_per_second; }

    // Serialization
    bool saveProjects() {
        #ifndef DROP_TARGET_HTML5
            std::ofstream os("out.cereal", std::ios::binary);
            cereal::BinaryOutputArchive archive(os);

            for (auto proj_pair : m_projects) {
                archive( proj_pair.second );
            }
        #endif

        return true;
    };

};


#endif  // DR_APP_H