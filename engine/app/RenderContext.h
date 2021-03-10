//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 08 2021
//
//
#ifndef DR_RENDER_CONTEXT_H
#define DR_RENDER_CONTEXT_H


// Shaders
#include "engine/scene3d/shaders/BasicShader.glsl.h"

    
//####################################################################################
//##    DrRenderContext
//##        Holds rendering / pipeline / bindings info 
//############################
class DrRenderContext 
{
public:
    // Constructor / Destructor
    DrRenderContext(DrApp* parent_app);
    ~DrRenderContext() { }

    // #################### VARIABLES ####################
public:
    // External Borrowed Pointers
    DrApp*              m_app                   { nullptr };                        // Pointer to the parent App

    // Render Context Variables
    sg_pass_action      pass_action     {};     
    sg_pipeline         pipeline        {};     // Shader... Pipeline holds shader, vertex shader attribute type, primitive type, index type, cull mode, depth info, blend mode
    sg_bindings         bindings        {};     // Mesh...   Bindings hold vertex buffers, index buffers, and fragment shader images


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions


};

#endif  // DR_RENDER_CONTEXT_H
