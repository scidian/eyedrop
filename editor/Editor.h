/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_EDITOR_H
#define DR_EDITOR_H

// Includes
#include "engine/app/App.h"
#include "editor/Types.h"

// Forward Declarations
class DrImage;

// Constants
#define DOCKSPACE_NAME          "EditorDockspace"
#define EDITOR_TOOLBAR_SIZE     40

// Gui Images
enum Editor_Images {
    EDITOR_IMAGE_NONE               = -1,                                           // Represents no image found / loaded / etc.
    EDITOR_IMAGE_WORLD_GRAPH,
    EDITOR_IMAGE_WORLD_CREATOR,
    EDITOR_IMAGE_UI_CREATOR,
    EDITOR_IMAGE_TOTAL,
};


//####################################################################################
//##    DrEditor
//##        EyedropGame Editor built on top of Eyedrop App Engine
//############################
class DrEditor : public DrApp
{
public:
    DrEditor(std::string title = "Eyedrop", DrColor bg_color = DROP_COLOR_BLACK, int width = 800, int height = 600);
    virtual ~DrEditor();


    // #################### VARIABLES ####################
public:
    std::vector<std::shared_ptr<DrImage>>           gui_images;                     // Images used with user interface



    // !!!!! #TEMP: Variables, used for demo
    std::shared_ptr<DrMesh>     m_mesh              { std::make_shared<DrMesh>() };
    std::shared_ptr<DrImage>    m_image             { nullptr };
    int                         m_mesh_quality      { 5 };

    // Image Variables
    int                         m_before_keys       { 5 };

    // Model Rotation
    DrVec2                      m_total_rotation    {  0.f,  0.f };
    DrVec2                      m_add_rotation      { 25.f, 25.f };
    std::vector<hmm_mat4>       m_model             { };
    DrVec2                      m_mouse_down        { 0, 0 };
    float                       m_rotate_speed      { 1.f };
    bool                        m_is_mouse_down     { false };
    float                       m_zoom              { 2.0f };
    bool                        m_wireframe         { false };
    // !!!!! End temp variables



    // #################### INTERNAL FUNCTIONS ####################
    // Override update functions
    virtual void onCreate() override;
    virtual void onUpdateScene() override;
    virtual void onUpdateGUI() override;
    virtual void onEvent(const sapp_event* event) override;


    // Temp Demo Functions
    void        calculateMesh(bool reset_position);
    void        resetPositions();

};


#endif  // DR_EDITOR_H