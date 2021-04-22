//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Tue Mar 02 2021
//
//
#include "engine/app/App.h"


//####################################################################################
//##    Player (engine runtime only)
//####################################################################################
class DrPlayer : public DrApp 
{
public:
    using DrApp::DrApp;                                                             // Inherits base constructor, requires C++ 11

    virtual void onUpdateGUI() override { 

        #if defined(ENABLE_IMGUI)  
            // Demo Window
            //ImGui::ShowDemoWindow();
        #endif

        // Playing with frame count
        uint64_t elapsed =  stm_since(m_time_start);
        double seconds =    stm_sec(elapsed);
        setAppName("Frame count: " + std::to_string(sapp_frame_count()));
    }

    virtual void onEvent(const sapp_event* event) override {
       
    }
};


int main(int argc, char* argv[]) {

    DrPlayer* player = new DrPlayer("Test Player", DROP_COLOR_PURPLE);
    player->run();

}