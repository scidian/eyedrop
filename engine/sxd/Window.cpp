//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 01 2021
//
//
#include <functional>
#include "Window.h"

// Namespace
namespace sxd {


//####################################################################################
//##    Local Structs / Defines
//####################################################################################
#define MAX_FILE_SIZE (1024 * 1024)

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
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;

    // Fetch / Drop
    uint8_t file_buffer[MAX_FILE_SIZE];
    loadstate_t load_state;

    // Events
    item_t items[_SAPP_EVENTTYPE_NUM];

    // Font
    //FONScontext* fons;
    //float   dpi_scale;
    //int     font_normal;
    //uint8_t font_normal_data[MAX_FILE_SIZE];
};


// ***** C callback wrappers for using member methods as callbacks functions
std::function<void()> onCreateCallback;     
std::function<void()> onUpdateCallback;     
std::function<void(const sapp_event* event)> onEventCallback;      
std::function<void()> onDestroyCallback;    
extern "C" void onCreateWrapper()   { onCreateCallback(); }
extern "C" void onUpdateWrapper()   { onUpdateCallback(); }
extern "C" void onEventWrapper(const sapp_event* event)    { onEventCallback(event); }
extern "C" void onDestroyWrapper()  { onDestroyCallback(); }


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
Window::Window(App *parent_app) : m_app(parent_app) {
    onCreateCallback = std::bind(&Window::onCreate, this);
    onUpdateCallback = std::bind(&Window::onUpdate, this);
    onEventCallback = std::bind(&Window::onEvent, this, std::placeholders::_1);
    onDestroyCallback = std::bind(&Window::onDestroy, this);
    m_sokol_app.window_title =  "ImGui Test";
    m_sokol_app.init_cb =       onCreateWrapper;    
    m_sokol_app.frame_cb =      onUpdateWrapper;
    m_sokol_app.event_cb =      onEventWrapper;
    m_sokol_app.cleanup_cb =    onDestroyWrapper;
    m_sokol_app.width =         800;
    m_sokol_app.height =        600;
    m_sokol_app.enable_clipboard = true;
    m_sokol_app.enable_dragndrop = true;
    m_sokol_app.max_dropped_files = 1;
}

Window::~Window() {
        
}


//####################################################################################
//##    Sokol App Events
//####################################################################################
void Window::onCreate(void) {
    // ***** Setup sokol-gfx, call sokol_glue function to obtain values from sokol_app
    sg_desc (sokol_gfx) {
        .context = sapp_sgcontext()
    };            
    sg_setup(&sokol_gfx);
}

void Window::onUpdate(void) {

}

void Window::onEvent(const sapp_event* event) {

}

void Window::onDestroy(void) {

}



}       // end namespace sxd;