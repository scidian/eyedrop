/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/

#include "engine/app/App.h"


//####################################################################################
//##    Player (engine runtime only)
//####################################################################################
class DrPlayer : public DrApp
{
public:
    using DrApp::DrApp;                                                             // Inherits base constructor, requires C++ 11

    virtual void onUpdateGUI() override {

        #if defined(DROP_IMGUI)
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