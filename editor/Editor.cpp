//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "core/geometry/Matrix.h"
#include "core/geometry/Rect.h"
#include "core/geometry/Vec2.h"
#include "core/imaging/Bitmap.h"
#include "core/imaging/Color.h"
#include "core/imaging/Filter.h"
#include "core/imaging/Image.h"
#include "core/Math.h"
#include "core/Random.h"
#include "../engine/app/App.h"
#include "../engine/app/sokol/Event__strings.h"
#include "../engine/scene3d/Mesh.h"
#include "../engine/scene3d/shaders/BasicShader.glsl.h"
#include "widgets/StyleSelector.h"


//####################################################################################
//##    Callback: File Dropped
//####################################################################################
#if defined(DROP_TARGET_HTML5)
    // The async-loading callback for sapp_html5_fetch_dropped_file
    static void emsc_load_callback(const sapp_html5_fetch_response* response) {
        DrApp* app = (DrApp*)(response->user_data);
        if (app) {
            if (response->succeeded) {
                app->loadImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
            } else {
                // File too big if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
            }
        }
    }
#else
    // the async-loading callback for native platforms
    static void native_load_callback(const sfetch_response_t* response) {
        DrApp* app = (DrApp*)(response->user_void_ptr);
        if (app) {
            if (response->fetched) {
                app->loadImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
            } else if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL) {
                // File too big if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
            }
        }
    }
#endif


//####################################################################################
//##    Editor
//####################################################################################
class Editor : public DrApp 
{
public:
    using DrApp::DrApp;                                                 // Inherits base constructor, requires C++ 11

    virtual void onCreate() override { }

    virtual void onUpdateScene() override { 
         // ***** Compute model-view-projection matrix for vertex shader
        hmm_mat4 proj = HMM_Perspective(52.5f, (float)sapp_width()/(float)sapp_height(), 5.f, 20000.0f);
        hmm_mat4 view = HMM_LookAt(HMM_Vec3(0.0f, 1.5f, m_mesh->image_size * m_zoom), HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
        hmm_mat4 view_proj = HMM_MultiplyMat4(proj, view);

        hmm_mat4 rxm = HMM_Rotate(m_add_rotation.x, HMM_Vec3(1.0f, 0.0f, 0.0f));
        hmm_mat4 rym = HMM_Rotate(m_add_rotation.y, HMM_Vec3(0.0f, 1.0f, 0.0f));
        hmm_mat4 rotate = HMM_MultiplyMat4(rxm, rym); 
        m_model =  HMM_MultiplyMat4(rotate, m_model);
        m_total_rotation.x = EqualizeAngle0to360(m_total_rotation.x + m_add_rotation.x);
        m_total_rotation.y = EqualizeAngle0to360(m_total_rotation.y + m_add_rotation.y);
        m_add_rotation.set(0.f, 0.f);

        // Uniforms for vertex shader
        vs_params_t vs_params;
        vs_params.m =   m_model;
        vs_params.mvp = HMM_MultiplyMat4(view_proj, m_model);
        
        // Uniforms for fragment shader
        fs_params_t fs_params;
        fs_params.u_wireframe = (m_mesh->wireframe) ? 1.0f : 0.0f;

        // Check if user requested new model quality, if so recalculate
        if (m_mesh_quality != m_before_keys) {
            calculateMesh(false);
            m_before_keys = m_mesh_quality;
        }

        sg_apply_pipeline(m_state.pip);
        sg_apply_bindings(&m_state.bind);
        sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
        sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
        sg_draw(0, m_mesh->indices.size(), 1);
    }

    virtual void onUpdateGUI() override { 

        // Menu
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                bool clicked_new, clicked_open, clicked_save;
                if (ImGui::MenuItem("New",  "CTRL+N", &clicked_new)) { }
                if (ImGui::MenuItem("Open", "CTRL+O", &clicked_open)) { }
                if (ImGui::MenuItem("Save", "CTRL+S", &clicked_save)) { }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "CTRL+Q")) { sapp_request_quit(); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Cut", "CTRL+X")) { }
                if (ImGui::MenuItem("Copy", "CTRL+C")) { }
                if (ImGui::MenuItem("Paste", "CTRL+V")) { }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                bool clicked_themes;
                ImGui::MenuItem("Style Selector", 0, &clicked_themes);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Create main background window for DockSpace
        bool open = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	    ImGuiViewport* viewport = ImGui::GetMainViewport();
	    ImGui::SetNextWindowPos(viewport->Pos);
	    ImGui::SetNextWindowSize(viewport->Size);
	    ImGui::SetNextWindowViewport(viewport->ID);
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);    
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	    ImGui::Begin("DockSpaceViewport", &open, window_flags);
	    ImGui::PopStyleVar(3);
	    
        // Programatically build dock space
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        if (ImGui::DockBuilderGetNode(dockspace_id) == NULL) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::DockBuilderRemoveNode(dockspace_id);                                                             // Clear out existing layout
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlagsPrivate_::ImGuiDockNodeFlags_DockSpace);      // Add empty node
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            ImGuiID dock_main_id =      dockspace_id; 
            ImGuiID dock_id_top =       ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up,    0.15f, NULL, &dock_main_id);
            ImGuiID dock_id_left =      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left,  0.15f, NULL, &dock_main_id);
            ImGuiID dock_id_right =     ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.15f, NULL, &dock_main_id);
            ImGuiID dock_id_bottom =    ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down,  0.10f, NULL, &dock_main_id);

            ImGui::DockBuilderDockWindow("Assets",              dock_id_left);
            ImGui::DockBuilderDockWindow("Toolbar",             dock_id_top);               // ---> To apply to central node: dock_main_id);
            ImGui::DockBuilderDockWindow("Property Inspector",  dock_id_right);
            ImGui::DockBuilderDockWindow("Status Bar",          dock_id_bottom);
            ImGui::DockBuilderFinish(dockspace_id);
        }

        // Apply dockspace to main dock space viewport
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;// | ImGuiDockNodeFlags_AutoHideTabBar;
        dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.f,0.f,0.f,0.f));
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        ImGui::PopStyleColor();
        ImGui::End();

        // Widget Windows
        window_flags = ImGuiWindowFlags_NoCollapse;

        ImGui::Begin("Toolbar", &open, window_flags);
            ImGui::Text("Text 2");
        ImGui::End();

        ImGui::Begin("Assets", &open, window_flags);
            ImGui::Text("Asset 1");
            ImGui::Text("Asset 2");
            ImGui::Text("Asset 3");
            ImGui::Text("Asset 4");
        ImGui::End();
        ImGuiID assets_id = ImGui::GetID("Assets");
        //ImGui::SetWi


        ImGui::Begin("Property Inspector", &open, window_flags);
            static ImVec4 base = ImVec4(0.000f, 0.750f, 0.720f, 1.0f);
            static float f1 = 0.123f, f2 = 0.0f;
            ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
            static float angle = 0.0f;
            ImGui::SliderAngle("slider angle", &angle);
            ImGui::ColorEdit3("base", (float*) &base, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::End();

        ImGui::Begin("Status Bar", &open, window_flags);
            ImGui::Text("Some status text");
        ImGui::End();

        // Demo Window
        ImGui::ShowDemoWindow();

        // Theme selector
        ImGui::SetNextWindowContentSize(ImVec2(250, 250));
        ThemeSelector();
    }

    virtual void onEvent(const sapp_event* event) override {
        if ((event->type == SAPP_EVENTTYPE_KEY_DOWN) && !event->key_repeat) {
            switch (event->key_code) {
                case SAPP_KEYCODE_1: 
                case SAPP_KEYCODE_2:
                case SAPP_KEYCODE_3:
                case SAPP_KEYCODE_4:
                case SAPP_KEYCODE_5:
                case SAPP_KEYCODE_6:
                case SAPP_KEYCODE_7:
                case SAPP_KEYCODE_8:
                case SAPP_KEYCODE_9:
                    m_mesh_quality = event->key_code - SAPP_KEYCODE_1;
                    break;
                case SAPP_KEYCODE_R:
                    m_total_rotation.set(0.f, 0.f);
                    m_add_rotation.set(25.f, 25.f);
                    m_model = DrMatrix::identityMatrix();
                    break;
                case SAPP_KEYCODE_W:
                    m_mesh->wireframe = !m_mesh->wireframe;
                    m_wireframe = m_mesh->wireframe;
                    break;
                default: ;
            }
                    
        } else if (event->type == SAPP_EVENTTYPE_MOUSE_SCROLL) {
            m_zoom -= (event->scroll_y * 0.1f);
            m_zoom = Clamp(m_zoom, 0.5f, 5.0f);

        } else if (event->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
            if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
                m_mouse_down.set(event->mouse_y, event->mouse_x);
                m_is_mouse_down = true;
            }
            
        } else if (event->type == SAPP_EVENTTYPE_MOUSE_UP) {
            if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
                m_is_mouse_down = false;
            }

        } else if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
            if (m_is_mouse_down) {
                float x_movement = event->mouse_y;
                float y_movement = event->mouse_x;

                if (m_mouse_down.x < x_movement) {
                    m_add_rotation.x = m_rotate_speed * (x_movement - m_mouse_down.x);
                } else if (m_mouse_down.x > x_movement) {
                    m_add_rotation.x = 360 - (m_rotate_speed * (m_mouse_down.x - x_movement));
                }
                
                if (m_mouse_down.y > y_movement) {
                    m_add_rotation.y = 360 - (m_rotate_speed * (m_mouse_down.y - y_movement));
                } else if (m_mouse_down.y < y_movement) {
                    m_add_rotation.y = m_rotate_speed * (y_movement - m_mouse_down.y);
                }

                m_mouse_down.x = x_movement;
                m_mouse_down.y = y_movement;
                m_add_rotation.x = EqualizeAngle0to360(m_add_rotation.x);
                m_add_rotation.y = EqualizeAngle0to360(m_add_rotation.y);
            }

        } else if (event->type == SAPP_EVENTTYPE_FILES_DROPPED) {
            #if defined(DROP_TARGET_HTML5)
                // on emscripten need to use the sokol-app helper function to load the file data
                sapp_html5_fetch_request (sokol_fetch_request) {
                    .dropped_file_index = 0,
                    .buffer_ptr = m_state.file_buffer,
                    .buffer_size = sizeof(m_state.file_buffer),
                    .user_data = this,
                    .callback = emsc_load_callback,
                };
                sapp_html5_fetch_dropped_file(&sokol_fetch_request);
            #else
                // native platform: use sokol-fetch to load file content
                sfetch_request_t (sokol_fetch_request) {
                    .path = sapp_get_dropped_file_path(0),
                    .buffer_ptr = m_state.file_buffer,
                    .buffer_size = sizeof(m_state.file_buffer),
                    .user_void_ptr = this,
                    .callback = native_load_callback,
                };
                sfetch_send(&sokol_fetch_request);
            #endif
        }
    }
};


int main(int argc, char* argv[]) {

    Editor* editor = new Editor("Test Editor", DrColor(28, 30, 29), 1800, 1000);
    editor->run();

}

