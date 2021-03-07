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
#include "../engine/app/sokol/Event__strings.h"
#include "../engine/scene3d/Mesh.h"
#include "Editor.h"
#include "Types.h"
#include "ui/Dockspace.h"
#include "ui/Menu.h"
#include "ui/Toolbar.h"
#include "widgets/ThemeSelector.h"


//####################################################################################
//##    Program Start
//####################################################################################
int main(int argc, char* argv[]) {

    DrEditor* editor = new DrEditor("Test Editor", DrColor(28, 30, 29), 1750, 1000);
    editor->run();

}


//####################################################################################
//##    Destructor
//####################################################################################
DrEditor::~DrEditor() { }


//####################################################################################
//##    On Create
//####################################################################################
void DrEditor::onCreate() { 

    for (int i = 0; i < EDITOR_IMAGE_TOTAL; i++) {
        images[i] = nullptr;
    }

    // Reference:
    // (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
    // std::string image_file = m_app_directory + "assets/blob.png";
    
    sfetch_request_t (sokol_fetch_image) {
        .path = (m_app_directory + "assets/toolbar_icons/toolbar_world_graph.png").c_str(),
        .buffer_ptr = m_state.file_buffer2,
        .buffer_size = sizeof(m_state.file_buffer2),
        .user_void_ptr = this,
        .callback = +[](const sfetch_response_t* response) {
            DrEditor* app = (DrEditor*)(response->user_void_ptr);
            if (response->fetched && app) {
                int png_width, png_height, num_channels;
                const int desired_channels = 4;
                stbi_uc* pixels = stbi_load_from_memory((stbi_uc *)response->buffer_ptr, (int)response->fetched_size,
                                                        &png_width, &png_height, &num_channels, desired_channels);

                // Stb Load Succeeded
                if (pixels) {
                    sg_image_desc img_desc { };
                        img_desc.width =    png_width;
                        img_desc.height =   png_height;
                        img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
                        img_desc.wrap_u =       SG_WRAP_CLAMP_TO_EDGE;
                        img_desc.wrap_v =       SG_WRAP_CLAMP_TO_EDGE;
                        img_desc.min_filter =   SG_FILTER_LINEAR;
                        img_desc.mag_filter =   SG_FILTER_LINEAR;
                        img_desc.data.subimage[0][0].ptr = pixels;
                        img_desc.data.subimage[0][0].size = static_cast<size_t>(png_width * png_height * num_channels);
                    app->images[EDITOR_IMAGE_WORLD_GRAPH] = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
                }
                
            }
        },       
    };
    sfetch_send(&sokol_fetch_image);    
}


//####################################################################################
//##    Render Update
//####################################################################################
void DrEditor::onUpdateScene() { 
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

#define HZ_CORE_IMGUI_COMPONENT_VAR(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

//####################################################################################
//##    Gui Update
//####################################################################################
void DrEditor::onUpdateGUI() { 
    // Keep track of open windows / widgets
    static bool widgets[EDITOR_WIDGET_TOTAL_NUMBER];
    if (m_first_frame) {
        std::fill(widgets, widgets + EDITOR_WIDGET_TOTAL_NUMBER, true);
        widgets[EDITOR_WIDGET_THEME] = false;
        widgets[EDITOR_WIDGET_STYLE] = false;
        widgets[EDITOR_WIDGET_DEMO] =  false;
    }
    
    // Menu
    MainMenuUI(widgets);
    
    // Handle Dockspace
    int menu_height = 0;
    DockspaceUI(widgets, menu_height);

    // Handle Toolbar
    ToolbarUI(widgets, images, menu_height);


    // ##### Widget Windows
    ImGuiWindowFlags child_flags = 0; //ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    
    // if (widgets[EDITOR_WIDGET_STATUS]) {
    //     ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 100));
    //     ImGui::Begin("Status Bar", &widgets[EDITOR_WIDGET_STATUS], child_flags);
    //         ImGui::Text("Some status text");
    //     ImGui::End();
    //     ImGui::PopStyleVar();
    // }
    
    if (widgets[EDITOR_WIDGET_ASSETS]) {
        ImGui::Begin("Assets", &widgets[EDITOR_WIDGET_ASSETS], child_flags);
            ImGui::Text("Asset 1");
            ImGui::Text("Asset 2");
            ImGui::Text("Asset 3");
            ImGui::Text("Asset 4");
        ImGui::End();
    }
    
    if (widgets[EDITOR_WIDGET_INSPECTOR]) {
        ImGui::Begin("Property Inspector", &widgets[EDITOR_WIDGET_INSPECTOR], child_flags);
            static ImVec4 base  = ImVec4(0.000f, 0.750f, 0.720f, 1.0f);
            static ImVec4 base2 = ImVec4(0.000f, 0.750f, 0.720f, 1.0f);
            static float f1 = 0.123f, f2 = 0.0f;
            static float angle = 0.0f;

            ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderAngle("slider angle", &angle);
            ImGui::Text("Base: "); ImGui::SameLine(); ImGui::ColorEdit3("##Base", (float*) &base, ImGuiColorEditFlags_PickerHueWheel);

            ImGui::BeginColumns("Cols", 2);
            {
                bool flag;
	            HZ_CORE_IMGUI_COMPONENT_VAR(ImGui::ColorEdit3("##Color 2", (float*) &base2), "Color 2", int i;);
                HZ_CORE_IMGUI_COMPONENT_VAR(ImGui::Checkbox("##Fixed Aspect Ratio", &flag), "Fixed Aspect Ratio", int j;);
            }
            ImGui::EndColumns();
        ImGui::End();
    }

    // Demo Window
    if (widgets[EDITOR_WIDGET_DEMO]) {
        ImGui::ShowDemoWindow();
    }

    // Style Selector
    if (widgets[EDITOR_WIDGET_STYLE]) {
        ImGui::ShowStyleEditor();
    }   

    // Theme selector
    if (widgets[EDITOR_WIDGET_THEME] || m_first_frame) {
        //ImGui::SetNextWindowPos(ImVec2(100, 200));
        ImGui::SetNextWindowContentSize(ImVec2(250, 250));
        ThemeSelectorUI(widgets[EDITOR_WIDGET_THEME], child_flags, m_first_frame);
    }
}


//####################################################################################
//##    Event Update
//####################################################################################
void DrEditor::onEvent(const sapp_event* event) {
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
                .callback = +[](const sfetch_response_t* response) {
                    DrApp* app = (DrApp*)(response->user_data);
                    if (app) {
                        if (response->succeeded) {
                            app->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
                        } else {
                            // File too big if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
                        }
                    }
                },
            };
            sapp_html5_fetch_dropped_file(&sokol_fetch_request);
        #else
            // native platform: use sokol-fetch to load file content
            sfetch_request_t (sokol_fetch_request) {
                .path = sapp_get_dropped_file_path(0),
                .buffer_ptr = m_state.file_buffer,
                .buffer_size = sizeof(m_state.file_buffer),
                .user_void_ptr = this,
                .callback = +[](const sfetch_response_t* response) {
                    DrApp* app = (DrApp*)(response->user_void_ptr);
                    if (app) {
                        if (response->fetched) {
                            app->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
                        } else {
                            // File too big if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
                        }
                    }
                },       
            };
            sfetch_send(&sokol_fetch_request);
        #endif
    }
}
