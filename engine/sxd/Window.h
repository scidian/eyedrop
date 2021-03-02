//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef SXD_WINDOW_H
#define SXD_WINDOW_H

// Includes
#include <string>
#include "3rd_party/sokol/sokol_app.h"
#include "3rd_party/sokol/sokol_gfx.h"
#include "3rd_party/sokol/sokol_glue.h"

// Namespace
namespace sxd {

// Forward Declarations
class App;


//####################################################################################
//##    Window
//##        Holds one Window for a Drop Application
//############################
class Window 
{
public:
    // Constructor / Destructor
    Window(App *parent_app);
    ~Window();

    // #################### VARIABLES ####################
private:
    // External Borrowed Pointers
    App            *m_app               { nullptr };                            // Holds reference to parent App

    // Window Variables
    std::string     m_window_title      { "Title" };                            // Window title
    int             m_width             { 800 };                                // Window width
    int             m_height            { 600 };                                // Window height

    // Local Variables
    sapp_desc       m_sokol_app;                                                // Sokol_app descriptor for this Window
    state_t         m_state;                                                    // Sokol_app state for this Window


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:
    void onCreate(void);
    void onUpdate(void);
    void onEvent(const sapp_event* event);
    void onDestroy(void);


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    int             getWidth()      { return m_width; }
    int             getHeight()     { return m_height; }
    std::string     getTitle()      { return m_window_title; }
    void            setTitle(std::string title) { m_window_title = title; }


};


}       // end namespace sxd;

#endif  // SXD_WINDOW_H