//
// Copyright (C) 2021 Scididan Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "App.h"

namespace sxd {

//####################################################################################
//##    Constructor / Destructor
//####################################################################################
App::App() {
    std::shared_ptr<Window> window = std::make_shared<Window>();
    m_windows.push_back(window);
}

App::~App() {
        
}



}       // end namespace sxd;