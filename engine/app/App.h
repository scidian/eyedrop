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

// STL Includes
#include <map>

// ##### 3rd Party
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/sokol/sokol_gl.h"
#if defined(DROP_DEBUG)
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
    #include "3rd_party/whereami.h"
#else
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#endif
#include "3rd_party/handmade_math.h"
#if defined(DROP_IMGUI)
    #include "3rd_party/imgui/imgui.h"
    #include "3rd_party/imgui/imgui_internal.h"
    //#include "3rd_party/icons_font_awesome5.h"                                    // If including font icons
    #include "3rd_party/sokol/sokol_imgui.h"
#endif

// ##### Engine
#include "engine/app/geometry/Matrix.h"
#include "engine/app/geometry/Vec2.h"
#include "engine/app/image/Color.h"
#include "engine/app/resources/ImageManager.h"
#include "engine/data/Constants.h"
#include "engine/data/Game.h"
#include "engine/data/Project.h"
#include "engine/scene3d/Mesh.h"

// Forward Declarations
class DrApp;
class DrImageManager;
class DrRenderContext;

//####################################################################################
//##    Global Variable Declarations
//####################################################################################
extern DrApp*               g_app_pointer;                                          // App singleton            Definition in App.cpp   Assigned in App.cpp::App()
DrApp*                      App();                                                  // Returns App singleton

//####################################################################################
//##    Local Structs / Defines
//############################
typedef std::map<std::string, std::shared_ptr<DrGame>>      GameMap;                // Holds data of open Game instances
typedef std::map<std::string, std::shared_ptr<DrProject>>   ProjectMap;             // Holds data of open Projects


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

    // #################### VARIABLES ####################
private:
    // Sokol Data
    sapp_desc               m_sokol_app;                                            // Sokol_app descriptor for this Window
    sg_features             m_sg_features;                                          // Sokol gfx runtime information about available optional features
    sg_limits               m_sg_limits;                                            // Sokol gfx runtime information about resource limits

    // Modules
    DrImageManager*         m_image_manager         { nullptr };                    // Image loading / atlas creation
    DrRenderContext*        m_context               { nullptr };                    // Rendering context for this App (currently built on Sokol_Gfx)
    
    // ----- User Data -----
    GameMap                 m_game                  { };                            // Collection of open Game instances
    ProjectMap              m_projects              { };                            // Collection of open Projects
    // ---------------------

    // App Variables
    std::string             m_app_name              { "" };                         // Name of Application   
    std::string             m_app_directory         { "" };                         // Root OS directory of application
    DrColor                 m_bg_color              { DROP_COLOR_BLACK };           // Background color of main App
    int                     m_width                 { 800 };                        // Window width
    int                     m_height                { 600 };                        // Window height
    float                   m_dpi_scale             { 1.f };                        // Dpi scale of device we're running on

    // Gui Fonts
    FONScontext*            m_fontstash;
    int                     m_font_normal = FONS_INVALID;

    // ImGui, disabled by default
    #if defined(DROP_DEBUG)
        sg_imgui_t          m_sg_imgui;                                             // Sokol_gfx_debug keeps track of data structures used by sokol_gfx for Debug View
    #endif

protected:
    // Time Variables
    bool                    m_first_frame       { true };                           // Turns false after first frame, allows for some initialization (colors, themeing, etc)
    uint64_t                m_time_start        { 0 };                              // Sokol_time start time since App started running
    double                  m_frames_per_second { 0.0 };                            // Stores current calculated frames per second

    
    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    virtual void    onCreate(void) { }
    virtual void    onUpdateScene(void) { }
    virtual void    onUpdateGUI(void) { }
    virtual void    onEvent(const sapp_event* event) { }
    virtual void    onDestroy(void) { }
    

    // #################### INTERNAL FUNCTIONS ####################
public:
    // Sokol Related
    void                run()       { sapp_run(m_sokol_app); }                      // Starts Sokol App
    const sg_features&  features()  { return m_sg_features; }                       // Return Sokol Gfx available optional features
    const sg_limits&    limits()    { return m_sg_limits; }                         // Return Sokol Gfx resource limits

    // Sokol Callbacks
    void    init(void);                                                             // Linked to internal sokol callbacks
    void    frame(void);                                                            // Linked to internal sokol callbacks
    void    event(const sapp_event* event);                                         // Linked to internal sokol callbacks
    void    cleanup(void);                                                          // Linked to internal sokol callbacks

    // Singletons
    DrImageManager*     imageManager()                                  { return m_image_manager; }
    DrRenderContext*    renderContext()                                 { return m_context; }
    
    // Local Variable Getters
    std::string         appName()                                       { return m_app_name; }
    std::string         appDirectory()                                  { return m_app_directory; }
    DrColor             backgroundColor()                               { return m_bg_color; }
    int                 getWidth() { return m_width; }
    int                 getHeight() { return m_height; }

    // Application Functions
    void                setAppName(std::string name);
    void                setAppIcon(const DrBitmap& icon);

    // Timer Functions
    bool                isFirstFrame()                                  { return m_first_frame; }
    double              framesPerSecond()                               { return m_frames_per_second; }
};


#endif  // DR_APP_H