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

// Includes
#include <map>
#include <string>
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/sokol/sokol_gfx_imgui.h"
#include "core/types/color.h"

// Forward Declarations
class FONScontext;


//####################################################################################
//##    Constants 
//############################
// Constants
const long      c_max_file_size = (1024 * 1024);
const int       c_start_app_key =  0;


//####################################################################################
//##    Local Structs / Defines
//############################
enum loadstate_t {
    LOADSTATE_UNKNOWN = 0,
    LOADSTATE_SUCCESS,
    LOADSTATE_FAILED,
    LOADSTATE_FILE_TOO_BIG,
};

struct item_t {
    sapp_event event = { };
};

struct state_t {
    // Gfx
    sg_pass_action  pass_action;
    sg_pipeline     pip;
    sg_bindings     bind;

    // Fetch / Drop
    uint8_t         file_buffer[c_max_file_size];
    loadstate_t     load_state;

    // Events
    item_t          items[_SAPP_EVENTTYPE_NUM];

    // Font
    FONScontext*    fons;
    float           dpi_scale;
    int             font_normal;
    uint8_t         font_normal_data[c_max_file_size];
};




//####################################################################################
//##    DrApp
//##        Base class of a Drop Application
//############################
class DrApp 
{
public:
    // Constructor / Destructor
    DrApp(std::string title, DrColor bg_color = DROP_COLOR_BLACK, int width = 800, int height = 600);
    ~DrApp();

    // #################### VARIABLES ####################
private:
    // App Variables
    std::string         m_app_name          { "" };                             // Name of Application
    
    

    // Local Variables
    long                m_key_generator     { c_start_app_key };                // Variable to hand out unique id key's to all child objects

    // Window Variables
    DrColor             m_bg_color          { DROP_COLOR_BLACK };               // Background color of main App
    int                 m_width             { 800 };                            // Window width
    int                 m_height            { 600 };                            // Window height

    // Local Variables
    sapp_desc           m_sokol_app;                                            // Sokol_app descriptor for this Window
    state_t             m_state;                                                // Sokol_app state for this Window
    sg_imgui_t          m_sg_imgui;                                             // Sokol_gfx_debug keeps track of data structures used by sokol_gfx
    uint64_t            m_time_start        { 0 };                              // Sokol_time start time since App started running


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    virtual void    onCreate(void) { }
    virtual void    onUpdate(void) { }
    virtual void    onEvent(const sapp_event* event) { }
    virtual void    onDestroy(void) { }

    

    // #################### INTERNAL FUNCTIONS ####################
public:
    // Start app
    void run() { sapp_run(m_sokol_app); }

    // Internal sokol callbacks
    void init(void);
    void frame(void);
    void event(const sapp_event* e);
    void cleanup(void);

    // Local Variable Functions
    std::string         appName()                                       { return m_app_name; }
    void                setAppName(std::string name) { m_app_name = name; }
    int                 getWidth() { return m_width; }
    int                 getHeight() { return m_height; }

    // Key Generator
    long                checkCurrentGeneratorKey()                      { return m_key_generator; }
    long                getNextKey()                                    { return m_key_generator++; }
    void                setGeneratorKeyStartNumber(long initial_key)    { m_key_generator = initial_key; }

};


#endif  // DR_APP_H