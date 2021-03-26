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
#include "core/Math.h"
#include "core/Random.h"
#include "core/Reflect.h"
#include "engine/app/file_system/LoadImage.h"
#include "engine/app/sokol/Event__strings.h"
#include "engine/app/App.h"
#include "engine/app/Image.h"
#include "engine/app/RenderContext.h"
#include "engine/scene3d/Mesh.h"
#include "ui/Dockspace.h"
#include "ui/Menu.h"
#include "ui/Toolbar.h"
#include "widgets/ThemeSelector.h"
#include "Editor.h"
#include "Types.h"


//####################################################################################
//##    Program Start
//####################################################################################
int main(int argc, char* argv[]) {

    // Turn on reflection
    InitializeReflection();

    // Create app and run
    DrEditor* editor = new DrEditor("Test Editor", DrColor(28, 30, 29), 1750, 1000);
    editor->run();

}


//####################################################################################
//##    Destructor
//####################################################################################
DrEditor::~DrEditor() { 
    for (auto& image : gui_images) { delete image; }
    gui_images.clear();
}


//####################################################################################
//##    On Create
//####################################################################################
void DrEditor::onCreate() { 

    // // #################### TESTING ####################
    // // ECS Construction
    // ecs()->registerComponent<Transform2D>();

    // // Create entity
    // int entity = ecs()->createEntity();
    // Transform2D et { };
    //     et.position =   std::vector<double>({1.0, 2.0, 3.0});
    //     et.rotation =   std::vector<double>({4.0, 5.0, 6.0});
    //     et.scale_xyz =  std::vector<double>({7.0, 8.0, 9.0});
    // ecs()->addComponent(entity, et);

    // // Component Iterate
    // Archetype entity_type = ecs()->getEntityType(entity);
    // std::cout << "Entity #" << entity << " has the following components:" << std::endl;
    // for (ComponentID id = 0; id < MAX_COMPONENTS; ++id) {
    //     std::cout << "Component #" << std::to_string(id) << "? ";
    //     if (entity_type.test(id)) {
    //         // Get Test
    //         HashID component_hash_id = ecs()->getComponentHashID(id);
    //         void*  component = ecs()->getData(id, entity);
    //         std::cout << "YES" << std::endl; 
    //         std::cout << "  Component Name: " << ClassData(ecs()->getComponentHashID(id)).name << std::endl;
    //         std::cout << "    First Variable: " << MemberData(ecs()->getComponentHashID(id), 0).name << std::endl;

    //         // Get Position
    //         TypeData member_data = MemberData(component_hash_id, "position");
    //         std::vector<double>& pos = ClassMember<std::vector<double>>(component, member_data);
    //         std::cout << "    " << member_data.title << " - X: " << pos[0] << ", Y: " << pos[1] << ", Z: " << pos[2] << std::endl;

    //         // Set Test
    //         pos = { 23.0, 43.2, 99.0 };

    //         // Check Set
    //         std::vector<double> check_pos = ClassMember<std::vector<double>>(component, member_data);
    //         std::cout << "  After setting - X: " << check_pos[0] << ", Y: " << check_pos[1] << ", Z: " << check_pos[2] << std::endl;
    //     } else {
    //         std::cout << "---" << std::endl;
    //     }
    // }

    // // Meta Data
    // std::cout << "Class Name:  " << ClassData<Transform2D>().name << std::endl;
    // std::cout << "Class Title: " << ClassData(et).title << std::endl;
    // std::cout << "Class Desc:  " << GetMetaData(ClassData("Transform2D"), META_DATA_DESCRIPTION) << std::endl;
    // std::cout << "Class Color: " << GetMetaData(ClassData("Transform2D"), META_DATA_COLOR) << std::endl;

    // DrColor clr(GetMetaData(ClassData("Transform2D"), META_DATA_COLOR));
    // std::cout << "Color Convert Back - R: " << std::to_string(clr.red())
    //                              << ", G: " << std::to_string(clr.green())
    //                              << ", B: " << std::to_string(clr.blue())
    //                              << ", A: " << std::to_string(clr.alpha())
    //                              << std::endl;

    // std::cout << "Prop Name:   " << MemberData<Transform2D>(0).name << std::endl;
    // std::cout << "Prop Title:  " << MemberData<Transform2D>(0).title << std::endl;
    // std::cout << "Prop Offset: " << MemberData(et, 0).offset << std::endl;
    
    // std::cout << "Prop Name:   " << MemberData<Transform2D>("rotation").name << std::endl;
    // std::cout << "Prop Title:  " << MemberData<Transform2D>(1).title << std::endl;
    // std::cout << "Prop Offset: " << MemberData(et, "rotation").offset << std::endl;

    // std::cout << "Prop Name:   " << MemberData<Transform2D>("scale_xyz").name << std::endl;
    // std::cout << "Prop Title:  " << MemberData<Transform2D>(2).title << std::endl;
    // std::cout << "Prop Offset: " << MemberData(et, "scale_xyz").offset << std::endl;

    // // EXAMPLE: Iterating Properties
    // std::cout << "Iterating Properties: " << std::endl;
    // for (int p = 0; p < ClassData("Transform2D").member_count; ++p) {
    //     std::cout << "  Property Number: " << p << ", Name: " << MemberData(et, p).name << std::endl;
    // }

    // // Test GetProperty by Index
    // std::vector<double> rotation = ClassMember<std::vector<double>>(&et, MemberData(et, 1));
    // std::cout << "Rotation X: " << rotation[0] << ", Rotation Y: " << rotation[1] << ", Rotation Z: " << rotation[2] << std::endl;
    
    // // Test GetProperty by Name
    // std::vector<double> position = ClassMember<std::vector<double>>(&et, MemberData(et, "position"));
    // std::cout << "Position X: " << position[0] << ", Position Y: " << position[1] << ", Position Z: " << position[2] << std::endl;

    // // Test SetProperty by Index
    // std::cout << "Transform 't' variable 'rotation.y' is currently: " << et.rotation[1] << std::endl;
    // std::cout << "  Setting Now..." << std::endl;
    // ClassMember<std::vector<double>>(&et, MemberData(et, "rotation"))[1] = 189;
    // std::cout << "  'rotation.y' is now: " << et.rotation[1] << std::endl;

    // // Test SetProperty by Name
    // ClassMember<std::vector<double>>(&et, MemberData(et, "position")) = { 56.0, 58.5, 60.2 };
    // std::cout << "Transform2D instance - Position X: " << et.position[0] << ", Position Y: " << et.position[1] << ", Position Z: " << et.position[2] << std::endl;

    // #############################################
    
    // Load Images
    for (int i = 0; i < EDITOR_IMAGE_TOTAL; ++i) gui_images.push_back(nullptr);
    AddImageToLoad(&gui_images[EDITOR_IMAGE_WORLD_GRAPH],    (appDirectory() + "assets/toolbar_icons/world_graph.png"));
    AddImageToLoad(&gui_images[EDITOR_IMAGE_WORLD_CREATOR],  (appDirectory() + "assets/toolbar_icons/world_creator.png"));
    AddImageToLoad(&gui_images[EDITOR_IMAGE_UI_CREATOR],     (appDirectory() + "assets/toolbar_icons/ui_creator.png"));


    // Initiate Blob Fetch
    AddImageToLoad(&m_image, appDirectory() + "assets/images/blob.png", true);
    //AddImageToLoad("http://github.com/stevinz/extrude/blob/master/assets/blob.png?raw=true");
}


//####################################################################################
//##    Render Update
//####################################################################################
void DrEditor::onUpdateScene() { 

    // Compute model-view-projection matrix for vertex shader
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
    if ((m_mesh_quality != m_before_keys) && (m_image != nullptr)) {
        calculateMesh(true);
        m_before_keys = m_mesh_quality;
    }

    sg_apply_pipeline(renderContext()->pipeline);
    sg_apply_bindings(&renderContext()->bindings);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    sg_draw(0, m_mesh->indices.size(), 1);
}


// Playing with column alignment
#define HZ_CORE_IMGUI_COMPONENT_VAR(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

//####################################################################################
//##    Gui Update
//####################################################################################
void DrEditor::onUpdateGUI() { 
    // Check for images to load
    FetchNextImage();

    // Keep track of open windows / widgets
    static bool widgets[EDITOR_WIDGET_TOTAL_NUMBER];
    if (isFirstFrame()) {
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
    ToolbarUI(widgets, gui_images, menu_height);


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
    if (widgets[EDITOR_WIDGET_THEME] || isFirstFrame()) {
        //ImGui::SetNextWindowPos(ImVec2(100, 200));
        ImGui::SetNextWindowContentSize(ImVec2(250, 250));
        ThemeSelectorUI(widgets[EDITOR_WIDGET_THEME], child_flags, isFirstFrame());
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
            sapp_html5_fetch_request sokol_fetch_request { };
                sokol_fetch_request.dropped_file_index = 0;
                sokol_fetch_request.buffer_ptr = m_drag_drop_file_buffer;
                sokol_fetch_request.buffer_size = sizeof(m_drag_drop_file_buffer);
                sokol_fetch_request.callback = +[](const sapp_html5_fetch_response* response) {
                    if (response->succeeded) {
                        DrEditor* editor = dynamic_cast<DrEditor*>(g_app);
                        editor->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
                    } else {
                        // File too big if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
                    }
                };
            sapp_html5_fetch_dropped_file(&sokol_fetch_request);
        #else
            // native platform: use sokol-fetch to load file content
            sfetch_request_t sokol_fetch_request { };
                sokol_fetch_request.path = sapp_get_dropped_file_path(0);
                sokol_fetch_request.buffer_ptr = m_drag_drop_file_buffer;
                sokol_fetch_request.buffer_size = sizeof(m_drag_drop_file_buffer);
                sokol_fetch_request.callback = +[](const sfetch_response_t* response) {
                    if (response->fetched) {
                        DrEditor* editor = dynamic_cast<DrEditor*>(g_app);
                        editor->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
                    } else {
                        // File too big if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
                    }                   
                };
            sfetch_send(&sokol_fetch_request);
        #endif
    }
}



//####################################################################################
//##    Create 3D extrusion
//####################################################################################
void DrEditor::calculateMesh(bool reset_position) {
    //##    Level of Detail:
    //##        0.075 = Detailed
    //##        0.250 = Nice
    //##        1.000 = Low poly
    //##       10.000 = Really low poly
    float level_of_detail = 0.6f;
    switch (m_mesh_quality) {
        case 0: level_of_detail = 19.200f;  break;
        case 1: level_of_detail =  9.600f;  break;
        case 2: level_of_detail =  4.800f;  break;
        case 3: level_of_detail =  2.400f;  break;
        case 4: level_of_detail =  1.200f;  break;
        case 5: level_of_detail =  0.600f;  break;
        case 6: level_of_detail =  0.300f;  break;
        case 7: level_of_detail =  0.150f;  break;
        case 8: level_of_detail =  0.075f;  break;
    }

    // During debug, To force init
    //
    // sg_image_desc sokol_image { };
    //         sokol_image.width =        m_image->bitmap().width;
    //         sokol_image.height =       m_image->bitmap().height;
    //         sokol_image.pixel_format = SG_PIXELFORMAT_RGBA8;
    //         sokol_image.wrap_u =       SG_WRAP_MIRRORED_REPEAT;
    //         sokol_image.wrap_v =       SG_WRAP_MIRRORED_REPEAT;
    //         sokol_image.min_filter =   SG_FILTER_LINEAR;
    //         sokol_image.mag_filter =   SG_FILTER_LINEAR;
    //         sokol_image.data.subimage[0][0].ptr =  &(m_image->bitmap().data[0]);
    //         sokol_image.data.subimage[0][0].size = (size_t)m_image->bitmap().size();
    // sg_init_image(renderContext()->bindings.fs_images[SLOT_tex], &sokol_image);


    // ***** Initialize Mesh
    m_image->outlinePoints(level_of_detail);
    m_mesh = std::make_shared<DrMesh>();
    m_mesh->image_size = Max(m_image->bitmap().width, m_image->bitmap().height);      
    m_mesh->wireframe = m_wireframe;
    m_mesh->initializeExtrudedImage(m_image, m_mesh_quality);
    //mesh->initializeTextureQuad();
    //mesh->initializeTextureCube();
    //mesh->initializeTextureCone();
            
    // ***** Copy vertex data and set into state buffer
    if (m_mesh->vertexCount() > 0) {
        // ***** Vertex Buffer
        unsigned int total_vertices = m_mesh->vertices.size();

        std::vector<Vertex> vertices(total_vertices);
        for (size_t i = 0; i < total_vertices; i++) vertices[i] = m_mesh->vertices[i];
        sg_buffer_desc sokol_buffer_vertex { };
            sokol_buffer_vertex.data = sg_range{ &vertices[0], vertices.size() * sizeof(Vertex) };
            sokol_buffer_vertex.label = "extruded-vertices";
        sg_destroy_buffer(renderContext()->bindings.vertex_buffers[0]);
        renderContext()->bindings.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

        // ***** Index Buffer
        unsigned int total_indices = m_mesh->indices.size();
        std::vector<uint16_t> indices(total_indices);
        for (size_t i = 0; i < total_indices; i++) indices[i] = m_mesh->indices[i];
        sg_buffer_desc sokol_buffer_index { };
            sokol_buffer_index.type = SG_BUFFERTYPE_INDEXBUFFER;
            sokol_buffer_index.data = sg_range{ &indices[0], indices.size() * sizeof(uint16_t) };
            sokol_buffer_index.label = "temp-indices";
        sg_destroy_buffer(renderContext()->bindings.index_buffer);
        renderContext()->bindings.index_buffer = sg_make_buffer(&(sokol_buffer_index));

        // ***** Reset rotation
        if (reset_position) {
            m_total_rotation.set(0.f, 0.f);
            m_add_rotation.set(25.f, 25.f);
            m_model = DrMatrix::identityMatrix();
        }
    }
}


void DrEditor::initImage(stbi_uc* buffer_ptr, int fetched_size) {
    int png_width, png_height, num_channels;
    const int desired_channels = 4;
    stbi_uc* pixels = stbi_load_from_memory(buffer_ptr, fetched_size, &png_width, &png_height, &num_channels, desired_channels);

    // Stb Load Succeeded
    if (pixels) {
        // ********** Copy data into our custom bitmap class, create image and trace outline
        DrBitmap bitmap(pixels, static_cast<int>(png_width * png_height * 4), false, png_width, png_height);
        //bitmap = Dr::ApplySinglePixelFilter(DROP_IMAGE_FILTER_HUE, bitmap, Dr::RandomInt(-100, 100));
        if (m_image != nullptr) delete m_image;
        m_image = new DrImage("shapes", bitmap, true);

        calculateMesh(true);

        // ********** Initialze the sokol-gfx texture
        sg_image_desc sokol_image { };
            sokol_image.width =        m_image->bitmap().width;
            sokol_image.height =       m_image->bitmap().height;
            sokol_image.pixel_format = SG_PIXELFORMAT_RGBA8;
            sokol_image.wrap_u =       SG_WRAP_MIRRORED_REPEAT;
            sokol_image.wrap_v =       SG_WRAP_MIRRORED_REPEAT;
            sokol_image.min_filter =   SG_FILTER_LINEAR;
            sokol_image.mag_filter =   SG_FILTER_LINEAR;
            sokol_image.data.subimage[0][0].ptr =  &(m_image->bitmap().data[0]);
            sokol_image.data.subimage[0][0].size = (size_t)m_image->bitmap().size();
        
        // To store an image onto the gpu:
        long image_id = sg_make_image(&sokol_image).id;

        // Initialize new image into shader bindings
        if (sg_query_image_info(renderContext()->bindings.fs_images[SLOT_tex]).slot.state == SG_RESOURCESTATE_VALID) {
            sg_uninit_image(renderContext()->bindings.fs_images[SLOT_tex]);
        }
        sg_init_image(renderContext()->bindings.fs_images[SLOT_tex], &sokol_image);

        stbi_image_free(pixels);
    }
}


