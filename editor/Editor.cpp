//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "3rd_party/handmade_math.h"
#include "3rd_party/imgui/imgui.h"
#include "3rd_party/stb/stb_image.h"
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


//####################################################################################
//##    Sokol File Scope Stuff
//####################################################################################
// Local reference to DrApp Singleton for Sokol App
DrApp* l_editor = nullptr;          


static ImVec4 base = ImVec4(0.000f, 0.700f, 0.650f, 1.0f);
static ImVec4 bg   = ImVec4(0.090f, 0.120f, 0.115f, 1.0f);
static ImVec4 text = ImVec4(0.900f, 0.930f, 0.925f, 1.0f);
static float high_val =         0.65f;
static float mid_val =          0.50f;
static float low_val =          0.35f;
static float window_offset =   -0.05f;

inline ImVec4 make_high(float alpha) {
    ImVec4 res(0, 0, 0, alpha);
    ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
    res.z = high_val;
    ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
    return res;
}

inline ImVec4 make_mid(float alpha) {
    ImVec4 res(0, 0, 0, alpha);
    ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
    res.z = mid_val;
    ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
    return res;
}

inline ImVec4 make_low(float alpha) {
    ImVec4 res(0, 0, 0, alpha);
    ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
    res.z = low_val;
    ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
    return res;
}

inline ImVec4 make_bg(float alpha, float offset = 0.f) {
    ImVec4 res(0, 0, 0, alpha);
    ImGui::ColorConvertRGBtoHSV(bg.x, bg.y, bg.z, res.x, res.y, res.z);
    res.z += offset;
    ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
    return res;
}

inline ImVec4 make_text(float alpha) {
    return ImVec4(text.x, text.y, text.z, alpha);
}

void theme_generator() {
    ImGui::Begin("Theme generator");
    ImGui::ColorEdit3("base", (float*) &base, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::ColorEdit3("bg", (float*) &bg, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::ColorEdit3("text", (float*) &text, ImGuiColorEditFlags_PickerHueWheel);
    ImGui::SliderFloat("high", &high_val, 0, 1);
    ImGui::SliderFloat("mid", &mid_val, 0, 1);
    ImGui::SliderFloat("low", &low_val, 0, 1);
    ImGui::SliderFloat("window", &window_offset, -0.4f, 0.4f);

    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = make_text(0.78f);
    style.Colors[ImGuiCol_TextDisabled]          = make_text(0.28f);
    style.Colors[ImGuiCol_WindowBg]              = make_bg(1.00f, window_offset);
    style.Colors[ImGuiCol_ChildBg]               = make_bg(0.58f);
    style.Colors[ImGuiCol_PopupBg]               = make_bg(0.9f);
    style.Colors[ImGuiCol_Border]                = make_bg(0.6f, -0.6f);
    style.Colors[ImGuiCol_BorderShadow]          = make_bg(0.0f, -0.8f);
    style.Colors[ImGuiCol_FrameBg]               = make_bg(1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = make_mid(0.78f);
    style.Colors[ImGuiCol_FrameBgActive]         = make_mid(1.00f);
    style.Colors[ImGuiCol_TitleBg]               = make_low(1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = make_high(1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = make_bg(0.75f);
    style.Colors[ImGuiCol_MenuBarBg]             = make_bg(0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]           = make_bg(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = make_low(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = make_mid(0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = make_mid(1.00f);
    style.Colors[ImGuiCol_CheckMark]             = make_high(1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = make_bg(1.0f, .1f);
    style.Colors[ImGuiCol_SliderGrabActive]      = make_high(1.0f);
    style.Colors[ImGuiCol_Button]                = make_bg(1.0f, .2f);
    style.Colors[ImGuiCol_ButtonHovered]         = make_mid(1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = make_high(1.00f);
    style.Colors[ImGuiCol_Header]                = make_mid(0.76f);
    style.Colors[ImGuiCol_HeaderHovered]         = make_mid(0.86f);
    style.Colors[ImGuiCol_HeaderActive]          = make_high(1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = make_mid(0.78f);
    style.Colors[ImGuiCol_ResizeGripActive]      = make_mid(1.00f);
    style.Colors[ImGuiCol_PlotLines]             = make_text(0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = make_mid(1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = make_text(0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = make_mid(1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = make_mid(0.43f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = make_bg(0.73f);
    style.Colors[ImGuiCol_Tab]                   = make_bg(0.40f);
    style.Colors[ImGuiCol_TabHovered]            = make_high(1.00f);
    style.Colors[ImGuiCol_TabActive]             = make_mid(1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = make_bg(0.40f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = make_bg(0.70f);
    style.Colors[ImGuiCol_DockingPreview]        = make_high(0.30f);

    if (ImGui::Button("Export")) {
        ImGui::LogToTTY();
        ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;\n");
        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            const ImVec4& col = style.Colors[i];
            const char* name = ImGui::GetStyleColorName(i);
            ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n",
                           name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
        }
        ImGui::LogFinish();
    }
    ImGui::End();
}


//####################################################################################
//##    Callback: File Dropped
//####################################################################################
#if defined(DROP_TARGET_HTML5)
// the async-loading callback for sapp_html5_fetch_dropped_file
static void emsc_load_callback(const sapp_html5_fetch_response* response) {
    if (response->succeeded) {
        l_editor.m_state.load_state = LOADSTATE_SUCCESS;
        l_editor.loadImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
    } else if (SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL == response->error_code) {
        l_editor.m_state.load_state = LOADSTATE_FILE_TOO_BIG;
    } else {
        l_editor.m_state.load_state = LOADSTATE_FAILED;
    }
}
#else
// the async-loading callback for native platforms
static void native_load_callback(const sfetch_response_t* response) {
    if (response->fetched) {
        l_editor->m_state.load_state = LOADSTATE_SUCCESS;
        l_editor->loadImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
    } else if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL) {
        l_editor->m_state.load_state = LOADSTATE_FILE_TOO_BIG;
    } else {
        l_editor->m_state.load_state = LOADSTATE_FAILED;
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

    virtual void onCreate() override {
        l_editor = this;
    }

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
            //calculateMesh(false);
            //m_before_keys = m_mesh_quality;
        }

        sg_apply_pipeline(m_state.pip);
        sg_apply_bindings(&m_state.bind);
        sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
        sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
        sg_draw(0, m_mesh->indices.size(), 1);
    }

    virtual void onUpdateGUI() override { 
        // Theme selector
        theme_generator();
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
                    .callback = emsc_load_callback,
                    .buffer_ptr = m_state.file_buffer,
                    .buffer_size = sizeof(m_state.file_buffer),
                };
                sapp_html5_fetch_dropped_file(&sokol_fetch_request);
            #else
                // native platform: use sokol-fetch to load file content
                sfetch_request_t (sokol_fetch_request) {
                    .path = sapp_get_dropped_file_path(0),
                    .callback = native_load_callback,
                    .buffer_ptr = m_state.file_buffer,
                    .buffer_size = sizeof(m_state.file_buffer)
                };
                sfetch_send(&sokol_fetch_request);
            #endif
        }
    }
};


int main(int argc, char* argv[]) {

    Editor* editor = new Editor("Test Editor", DROP_COLOR_BLUE);
    editor->run();

}

