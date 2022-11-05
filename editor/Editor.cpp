/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "engine/app/core/Math.h"
#include "engine/app/core/Random.h"
#include "engine/app/core/Reflect.h"
#include "engine/app/geometry/Matrix.h"
#include "engine/app/geometry/Rect.h"
#include "engine/app/geometry/Vec2.h"
#include "engine/app/image/Bitmap.h"
#include "engine/app/image/Color.h"
#include "engine/app/image/Filter.h"
#include "engine/app/image/Image.h"
#include "engine/app/sokol/Event__strings.h"
#include "engine/app/App.h"
#include "engine/app/RenderContext.h"
#include "engine/scene3d/Mesh.h"
#include "ui/Dockspace.h"
#include "ui/Menu.h"
#include "ui/Toolbar.h"
#include "widgets/ThemeSelector.h"
#include "Editor.h"


//####################################################################################
//##    Program Start
//####################################################################################
int main(int argc, char* argv[]) {

    // Turn on reflection
    InitializeReflection();

    // Create app and run
    DrEditor* editor = new DrEditor("Drop Creator", DrColor(28, 30, 29), 1750, 1000);
    editor->run();

}


//####################################################################################
//##    Contructor / Destructor
//####################################################################################
DrEditor::DrEditor(std::string title, DrColor bg_color, int width, int height) :
    DrApp(title, bg_color, width, height)
{
    m_model.resize(INSTANCES);
    resetPositions();

}

DrEditor::~DrEditor() { }


//####################################################################################
//##    Callbacks
//####################################################################################
// Sets shader texture to passed in image texture
void setMeshTexture(std::shared_ptr<DrImage>& image) {
    DrEditor* editor = dynamic_cast<DrEditor*>(App());
    editor->calculateMesh(true);
    App()->renderContext()->bindings.fs_images[SLOT_tex].id = image->gpuID();
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
    imageManager()->fetchImage({gui_images[EDITOR_IMAGE_WORLD_GRAPH],   (appDirectory() + "assets/toolbar_icons/world_graph.png"),      ATLAS_TYPE_ENGINE, ATLAS_PADDING});
    imageManager()->fetchImage({gui_images[EDITOR_IMAGE_WORLD_CREATOR], (appDirectory() + "assets/toolbar_icons/world_creator.png"),    ATLAS_TYPE_ENGINE, ATLAS_PADDING});
    imageManager()->fetchImage({gui_images[EDITOR_IMAGE_UI_CREATOR],    (appDirectory() + "assets/toolbar_icons/ui_creator.png"),       ATLAS_TYPE_ENGINE, ATLAS_PADDING});


    // Initiate Blob Fetch
    imageManager()->fetchImage({m_image, appDirectory() + "assets/images/blob.png",  ATLAS_TYPE_3D_GAME, ATLAS_PADDING, setMeshTexture, false});
    imageManager()->fetchImage({m_image, appDirectory() + "assets/images/craft.png", ATLAS_TYPE_3D_GAME, ATLAS_PADDING, setMeshTexture, false});
    //imageManager()->fetchImage({m_image, "http://github.com/stevinz/extrude/blob/master/assets/blob.png?raw=true", ATLAS_TYPE_3D_GAME, ATLAS_PADDING, setMeshTexture, false});
}


//####################################################################################
//##    Render Update
//####################################################################################
void DrEditor::onUpdateScene() {

    // Compute model-view-projection matrix for vertex shader
    hmm_mat4 proj = HMM_Perspective(52.5f, (float)sapp_width()/(float)sapp_height(), 5.f, 20000.0f);
    hmm_vec3 eye =  HMM_Vec3(0.0f, 1.5f, m_mesh->image_size * m_zoom);
    hmm_mat4 view = HMM_LookAt(eye, HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 view_proj = HMM_MultiplyMat4(proj, view);

    hmm_mat4 rxm = HMM_Rotate(m_add_rotation.x, HMM_Vec3(1.0f, 0.0f, 0.0f));
    hmm_mat4 rym = HMM_Rotate(m_add_rotation.y, HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 rotate = HMM_MultiplyMat4(rxm, rym);
    for (auto& m : m_model) {
        m = HMM_MultiplyMat4(rotate, m);
    }
    m_total_rotation.x = EqualizeAngle0to360(m_total_rotation.x + m_add_rotation.x);
    m_total_rotation.y = EqualizeAngle0to360(m_total_rotation.y + m_add_rotation.y);
    m_add_rotation.set(0.f, 0.f);

    // Uniforms for vertex shader
    vs_params_t vs_params { };
        vs_params.vp =  view_proj;

    // Uniforms for fragment shader
    fs_params_t fs_params { };
        fs_params.u_eye = eye;
        fs_params.u_premultiplied = 1.0f;
        fs_params.u_wireframe = (m_mesh->wireframe) ? 1.0f : 0.0f;

    // Check if user requested new model quality, if so recalculate
    if ((m_mesh_quality != m_before_keys) && (m_image != nullptr)) {
        calculateMesh(true);
        m_before_keys = m_mesh_quality;
    }

    if (m_image == nullptr) return;

    // Update Instance Buffers
    std::vector<hmm_mat4> instance_m(INSTANCES);
    for (int i = 0; i < INSTANCES; i++) {
        instance_m[i] = m_model[i];
    }
    sg_range instance_m_range {};
        instance_m_range.ptr = &instance_m[0];
        instance_m_range.size = (size_t)INSTANCES * sizeof(hmm_mat4);
    sg_update_buffer(renderContext()->bindings.vertex_buffers[1], instance_m_range);

    sg_apply_pipeline( renderContext()->pipeline);
    sg_apply_bindings(&renderContext()->bindings);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    sg_draw(0, m_mesh->indices.size(), INSTANCES);
}


// Playing with column alignment
#define HZ_CORE_IMGUI_COMPONENT_VAR(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

//####################################################################################
//##    Gui Update
//####################################################################################
void DrEditor::onUpdateGUI() {
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
                resetPositions();
                break;
            case SAPP_KEYCODE_W:
                m_mesh->wireframe = !m_mesh->wireframe;
                m_wireframe = m_mesh->wireframe;
                break;
            default: ;
        }

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_SCROLL) {
        m_zoom -= (event->scroll_y * 0.1f);
        m_zoom = Clamp(m_zoom, 0.5f, 25.0f);

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            m_mouse_down.set(event->mouse_y, event->mouse_x);
            m_is_mouse_down = true;
        }
     } else if (event->type == SAPP_EVENTTYPE_TOUCHES_BEGAN) {
        if (event->num_touches >= 0) {
            m_mouse_down.set(event->touches[0].pos_y, event->touches[0].pos_x);
            m_is_mouse_down = true;
        }

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_UP) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            m_is_mouse_down = false;
        }
    } else if (event->type == SAPP_EVENTTYPE_TOUCHES_ENDED) {
        if (event->num_touches == 0) m_is_mouse_down = false;

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE || event->type == SAPP_EVENTTYPE_TOUCHES_MOVED) {
        if (m_is_mouse_down) {
            float x_movement = 0.f;
            float y_movement = 0.f;

            if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
                x_movement = event->mouse_y;
                y_movement = event->mouse_x;
            } else if (event->type == SAPP_EVENTTYPE_TOUCHES_MOVED) {
                x_movement = event->touches[0].pos_y;
                y_movement = event->touches[0].pos_x;
            }

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
        // Load image, apply to mesh and shader afterwards
        bool perform_outline = false;
        bool was_dropped = true;
        imageManager()->fetchImage({m_image, sapp_get_dropped_file_path(0), ATLAS_TYPE_3D_GAME, ATLAS_PADDING, setMeshTexture, perform_outline, was_dropped});
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

    // ***** Initialize Mesh
    if (m_image->outlineRequested()) m_image->outlinePoints(level_of_detail);
    m_mesh = std::make_shared<DrMesh>();


    // !!!!! #TEMP: Size off of atlas
    //m_mesh->image_size = m_image->bitmap().maxDimension();
    std::shared_ptr<DrAtlas>& atlas = imageManager()->atlasFromGpuID(m_image->gpuID());
    m_mesh->image_size = Max(atlas->width, atlas->height);
    // !!!!! END TEMP
    m_mesh->wireframe = m_wireframe;


    // ***** Create Mesh
    //m_mesh->initializeExtrudedImage(m_image.get(), m_mesh_quality);
    //m_mesh->initializeTextureQuad();
    m_mesh->initializeTextureCube();
    //m_mesh->initializeTextureCone();


    // ***** Optimize and smooth mesh
    m_mesh->optimizeMesh();
    // ----- Experimental, doesnt work great -----
    //m_mesh->smoothMesh();


    // ***** Copy vertex data and set into state buffer
    if (m_mesh->vertexCount() > 0) {
        // ***** Vertex Buffer
        unsigned int total_vertices = m_mesh->vertices.size();

        std::vector<Vertex> vertices(total_vertices);
        for (size_t i = 0; i < total_vertices; i++) vertices[i] = m_mesh->vertices[i];
        sg_buffer_desc sokol_buffer_vertex { };
            sokol_buffer_vertex.data = sg_range{ &vertices[0], vertices.size() * sizeof(Vertex) };
            sokol_buffer_vertex.label = "Vertices-Extruded";
        sg_destroy_buffer(renderContext()->bindings.vertex_buffers[0]);
        renderContext()->bindings.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

        //sg_update_buffer(renderContext()->bindings.vertex_buffers[0], sokol_buffer_vertex.data);

        // ***** Index Buffer
        unsigned int total_indices = m_mesh->indices.size();
        std::vector<uint16_t> indices(total_indices);
        for (size_t i = 0; i < total_indices; i++) indices[i] = m_mesh->indices[i];
        sg_buffer_desc sokol_buffer_index { };
            sokol_buffer_index.type = SG_BUFFERTYPE_INDEXBUFFER;
            sokol_buffer_index.data = sg_range{ &indices[0], indices.size() * sizeof(uint16_t) };
            sokol_buffer_index.label = "Indices-Extruded";
        sg_destroy_buffer(renderContext()->bindings.index_buffer);
        renderContext()->bindings.index_buffer = sg_make_buffer(&(sokol_buffer_index));

        // ***** Reset rotation
        if (reset_position) {
            m_total_rotation.set(0.f, 0.f);
            m_add_rotation.set(25.f, 25.f);
            resetPositions();
        }
    }
}


//####################################################################################
//##    Reset Model Positions
//####################################################################################
void DrEditor::resetPositions() {

    float spacing_x = -1.f * (7.f * INSTANCE_X);
    float spacing_y = -1.f * (7.f * INSTANCE_Y);
    float step_x = (abs(spacing_x) * 2.f) / INSTANCE_X;
    float step_y = (abs(spacing_y) * 2.f) / INSTANCE_Y;

    int index = 0;
    float x = spacing_x;
    for (int i = 0; i < INSTANCE_X; i++) {
        float y = spacing_y;

        for (int j = 0; j < INSTANCE_Y; j++) {
            // Translate
            hmm_mat4 model = HMM_Translate(HMM_Vec3(x, y, 0.f));

            // Scale between 0.001 and 0.01f
            float scale = (rand() % 10) / 1000.0f + 0.01;
                  scale *= 0.6f;
            model = HMM_MultiplyMat4(model, HMM_Scale(HMM_Vec3(scale, scale, scale)));

            // Rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rot_angle = (rand() % 360);
            model = HMM_MultiplyMat4(model, HMM_Rotate(rot_angle, HMM_Vec3(0.4f, 0.6f, 0.8f)));

            m_model[index] = model;
            y += step_y;
            index++;
        }
        x += step_x;
    }

    if (m_image == nullptr) return;

    std::vector<hmm_vec4> instance_uv(INSTANCES);
    for (int i = 0; i < INSTANCES; i++) {
        instance_uv[i].X = m_image->uv0().x;
        instance_uv[i].Y = m_image->uv1().x;
        instance_uv[i].Z = m_image->uv0().y;
        instance_uv[i].W = m_image->uv1().y;
    }
    sg_range instance_uv_range {};
        instance_uv_range.ptr = &instance_uv[0];
        instance_uv_range.size = (size_t)INSTANCES * sizeof(hmm_vec4);
    sg_update_buffer(renderContext()->bindings.vertex_buffers[2], instance_uv_range);

}

