//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "App.h"
#include "Window.h"

// Namespace
namespace sxd {

//####################################################################################
//##    Constructor / Destructor
//####################################################################################
App::App() {
    std::shared_ptr<Window> window = std::make_shared<Window>(this);
    m_windows[getNextKey()] = window;
}

App::~App() {
        
}


//####################################################################################
//##    Getters / Setters
//####################################################################################
void App::setActiveWindow(std::shared_ptr<Window> window) {
    for (auto window_pair : m_windows) {
        if (window_pair.second == window) {
            m_active_window = window_pair.first;
            break;
        }
    }
}


}       // end namespace sxd;