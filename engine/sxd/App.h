//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef SXD_APP_H
#define SXD_APP_H

// Includes
#include <map>
#include <string>

// Namespace
namespace sxd {

// Forward Declarations
class Window;

// Typedefs
typedef std::shared_ptr<Window> win_;

// Constants
const int c_start_window_key = 0;


//####################################################################################
//##    App
//##        Base class of a Drop Application
//############################
class App 
{
public:
    // Constructor / Destructor
    App();
    ~App();

    // #################### VARIABLES ####################
private:
    // App Variables
    std::string             m_app_name          { "" };                         // Name of Application
    int                     m_active_window     { 0 };                          // Index of active window
    std::map<int, win_>     m_windows           { };                            // List of application Windows
    
    // Local Variables
    long                    m_key_generator     { c_start_window_key };         // Variable to hand out unique id key's to all child objects (in this case Windows)


    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    std::string         appName()               { return m_app_name; }
    win_                activeWindow()          { return m_windows[m_active_window]; }
    void                setActiveWindow(win_ window);
    void                setAppName(std::string name) { m_app_name = name; }

    // Window Key Generator
    long                checkCurrentGeneratorKey()                      { return m_key_generator; }
    long                getNextKey()                                    { return m_key_generator++; }
    void                setGeneratorKeyStartNumber(long initial_key)    { m_key_generator = initial_key; }

};

}       // end namespace sxd;


#endif  // SXD_APP_H