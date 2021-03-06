//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#ifndef DR_EDITOR_H
#define DR_EDITOR_H

#include "../engine/application/App.h"

// Constants
#define DOCKSPACE_NAME  "EditorDockspace"


//####################################################################################
//##    Editor
//####################################################################################
class DrEditor : public DrApp 
{
public:
    // Inherits base ctor, requires C++ 11
    using DrApp::DrApp;                                                 
    virtual ~DrEditor();

    // Override update functions
    virtual void onCreate() override;
    virtual void onUpdateScene() override;
    virtual void onUpdateGUI() override;
    virtual void onEvent(const sapp_event* event) override;
};


#endif  // DR_EDITOR_H