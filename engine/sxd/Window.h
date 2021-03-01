//
// Copyright (C) 2021 Scididan Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef SXD_WINDOW_H
#define SXD_WINDOW_H

#include <string>

namespace sxd {

//####################################################################################
//##    Window
//##        Holds one Window for a Drop Application
//############################
class Window {

private:
    std::string     m_window_title =    "Title";            // Window title
    int             m_width =           800;                // Window width
    int             m_height =          600;                // Window height

public:



};


}       // end namespace sxd;

#endif  // SXD_WINDOW_H