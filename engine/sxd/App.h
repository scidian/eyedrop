//
// Copyright (C) 2021 Scididan Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef SXD_APP_H
#define SXD_APP_H

#include <string>
#include <vector>
#include "Window.h"


namespace sxd {

using namespace std;

//####################################################################################
//##    App
//##        Base class of a Drop Application
//############################
class App {

    private:
        string                      m_app_name          { "" };             // Name of Application
        int                         m_active_window     { 0 };              // Index of active window
        vector<shared_ptr<Window>>  m_windows           { };                // List of application Windows
    
    public:
        App();
        ~App();

        

};

}       // end namespace sxd;


#endif  // SXD_APP_H