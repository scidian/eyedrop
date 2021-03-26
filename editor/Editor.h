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

#include "engine/app/App.h"

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
//##    Editor Image Handling
//############################
void        AddImageToLoad(Editor_Images image_number, std::string image_file);
void        FetchNextImage();



//####################################################################################
//##    Editor
//####################################################################################
class DrEditor : public DrApp 
{
public:
    using DrApp::DrApp;                                                             // Inherit base ctor, requires C++ 11
    virtual ~DrEditor() { }


    // #################### VARIABLES ####################
public:
    std::vector<std::shared_ptr<DrImage>>           gui_images;     



    // !!!!! #TEMP: Variables, used for demo
    std::shared_ptr<DrMesh>     m_mesh              { std::make_shared<DrMesh>() };
    std::shared_ptr<DrImage>    m_image             { nullptr };  
    int                         m_mesh_quality      { 5 };

    // Image Variables
    int                         m_before_keys       { m_mesh_quality };

    // Model Rotation
    DrVec2                      m_total_rotation    {  0.f,  0.f };
    DrVec2                      m_add_rotation      { 25.f, 25.f };
    hmm_mat4                    m_model             { DrMatrix::identityMatrix() };
    DrVec2                      m_mouse_down        { 0, 0 };
    float                       m_rotate_speed      { 1.f };
    bool                        m_is_mouse_down     { false };
    float                       m_zoom              { 1.5f };
    bool                        m_wireframe         { true };
    // !!!!! End temp variables



    // #################### INTERNAL FUNCTIONS ####################
    // Override update functions
    virtual void onCreate() override;
    virtual void onUpdateScene() override;
    virtual void onUpdateGUI() override;
    virtual void onEvent(const sapp_event* event) override;


    // Temp Demo Functions
    void        calculateMesh(bool reset_position);
    void        initImage(stbi_uc* buffer_ptr, int fetched_size);

};


#endif  // DR_EDITOR_H