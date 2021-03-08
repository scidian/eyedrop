//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 08 2021
//
//
#include "App.h"
#include "RenderContext.h"


//####################################################################################
//##    Blend Functions
//####################################################################################
// Normal
sg_blend_state (sokol_blend_normal) {
    .enabled =              true,
    .src_factor_rgb =       SG_BLENDFACTOR_ONE,
    .dst_factor_rgb =       SG_BLENDFACTOR_ZERO,
    .op_rgb =               SG_BLENDOP_ADD,
    .src_factor_alpha =     SG_BLENDFACTOR_ONE,
    .dst_factor_alpha =     SG_BLENDFACTOR_ZERO,
    .op_alpha =             SG_BLENDOP_ADD,
};

// Alpha Enabled
sg_blend_state (sokol_blend_alpha) {
    .enabled =              true,
    .src_factor_rgb =       SG_BLENDFACTOR_SRC_ALPHA,
    .dst_factor_rgb =       SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    .op_rgb =               SG_BLENDOP_ADD,
    .src_factor_alpha =     SG_BLENDFACTOR_SRC_ALPHA,
    .dst_factor_alpha =     SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    .op_alpha =             SG_BLENDOP_ADD,
};


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrRenderContext::DrRenderContext(DrApp* parent_app) : m_app(parent_app) {
    // Get clear color from parent DrApp
    DrColor color = parent_app->backgroundColor();
    float red =     static_cast<float>(color.redF());
    float green =   static_cast<float>(color.greenF());
    float blue =    static_cast<float>(color.blueF());
    float alpha =   static_cast<float>(color.redF());

    // Pass Action (action at start of render)
    // The default action for all pass attachments is SG_ACTION_CLEAR, with the clear color rgba = {0.5f, 0.5f, 0.5f, 1.0f}, depth=1.0 and stencil=0
    pass_action.colors[0].action = SG_ACTION_CLEAR;
    pass_action.colors[0].value = { red, green, blue, alpha };

    // Pipeline State Object
    sg_pipeline_desc (sokol_pipleine) { };
        sokol_pipleine.shader = sg_make_shader(basic_shader_shader_desc(sg_query_backend()));
        sokol_pipleine.layout.attrs[ATTR_vs_pos].format =       SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.layout.attrs[ATTR_vs_norm].format =      SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.layout.attrs[ATTR_vs_texcoord0].format = SG_VERTEXFORMAT_FLOAT2; //SG_VERTEXFORMAT_SHORT2N;
        sokol_pipleine.layout.attrs[ATTR_vs_bary].format =      SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
        //sokol_pipleine.index_type =   SG_INDEXTYPE_NONE;
        sokol_pipleine.index_type =     SG_INDEXTYPE_UINT16;
        //sokol_pipleine.cull_mode =    SG_CULLMODE_NONE; 
        sokol_pipleine.cull_mode =      SG_CULLMODE_FRONT;
        sokol_pipleine.depth.compare =  SG_COMPAREFUNC_LESS_EQUAL;
        sokol_pipleine.depth.write_enabled = true;
        sokol_pipleine.label = "BasicShader-Pipeline";
        sokol_pipleine.colors[0].blend = sokol_blend_alpha;
    pipeline = sg_make_pipeline(&sokol_pipleine);

    // ***** Allocate an image handle, 
    //  but don't actually initialize the image yet, this happens later when the asynchronous file load has finished.
    //  Any draw calls containing such an "incomplete" image handle will be silently dropped.
    bindings.fs_images[SLOT_tex] = sg_alloc_image();

    // ***** Starter triangle
    // Vertex buffer
    const Vertex vertices[] = {
        // pos                  normals                uvs          barycentric (wireframe)
        { -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      0,   0,      1.0f, 1.0f, 1.0f },
        {  1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      1,   0,      1.0f, 1.0f, 1.0f },
        {  1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      1,   1,      1.0f, 1.0f, 1.0f },      
        { -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      0,   1,      1.0f, 1.0f, 1.0f },      
    };
    sg_buffer_desc (sokol_buffer_vertex) {
        .data = SG_RANGE(vertices),
        .label = "temp-vertices"
    };
    bindings.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

    // Index buffer
    const uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };
    sg_buffer_desc (sokol_buffer_index) {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "temp-indices"
    };
    bindings.index_buffer = sg_make_buffer(&(sokol_buffer_index));
}

