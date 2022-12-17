/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "App.h"
#include "RenderContext.h"


//####################################################################################
//##    Blend Functions
//####################################################################################
/*
    case Blend_Object::Standard:
        ///glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                   // Standard non-premultiplied alpha blend
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);                            // Premultiplied alpha blend
    case Blend_Object::Additive:
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
    case Blend_Object::Subtractive:
        glBlendFuncSeparate(GL_SRC_COLOR, GL_DST_COLOR, GL_ZERO, GL_ONE);       // Subtract color only, not alpha
        glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);                              // Reverse Subtract is Dest - Source = new Color
*/

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

// Alpha Enabled
sg_blend_state (sokol_blend_premultipied_alpha) {
    .enabled =              true,
    .src_factor_rgb =       SG_BLENDFACTOR_ONE,
    .dst_factor_rgb =       SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    .op_rgb =               SG_BLENDOP_ADD,
    .src_factor_alpha =     SG_BLENDFACTOR_ONE,
    .dst_factor_alpha =     SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
    .op_alpha =             SG_BLENDOP_ADD,
};


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrRenderContext::DrRenderContext(DrColor initial_color) {
    // Set initial background color
    float red =     static_cast<float>(initial_color.redF());
    float green =   static_cast<float>(initial_color.greenF());
    float blue =    static_cast<float>(initial_color.blueF());
    float alpha =   static_cast<float>(initial_color.redF());

    // Pass Action (action at start of render)
    // The default action for all pass attachments is SG_ACTION_CLEAR, with the clear color rgba = {0.5f, 0.5f, 0.5f, 1.0f}, depth=1.0 and stencil=0
    pass_action.colors[0].action = SG_ACTION_CLEAR;
    pass_action.colors[0].value = { red, green, blue, alpha };

    // ***** Allocate an image handle,
    //  But don't actually initialize the image yet, this happens later when the asynchronous file load has finished.
    //  Any draw calls containing such an "incomplete" image handle will be silently dropped.
    bindings.fs_images[SLOT_tex] = sg_alloc_image();
    //  When ready to load an image into allocated image, call:
    /*
        uint32_t image_id = bindings.fs_images[SLOT_tex].id;
        sg_init_image(sg_image(image_id), &sokol_image);
    */
    //  To check if an id has been loaded into already and needs to be uninitialized, do:
    /*
        if (sg_query_image_info(sg_image(image_id)).slot.state == SG_RESOURCESTATE_VALID)
            sg_uninit_image(sg_image(image_id));
    */

    // ***** Starter triangle
    // Vertex buffer for static geometry (goes into vertex buffer bind slot 0)
    const Vertex vertices[] = {
        // pos                  normals                uvs          barycentric (wireframe)
        { -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      0,   0,      1.0f, 1.0f, 1.0f },
        {  1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      1,   0,      1.0f, 1.0f, 1.0f },
        {  1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      1,   1,      1.0f, 1.0f, 1.0f },
        { -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,      0,   1,      1.0f, 1.0f, 1.0f },
    };
    sg_buffer_desc sokol_buffer_vertex { };
        sokol_buffer_vertex.data = SG_RANGE(vertices);
        sokol_buffer_vertex.label = "Vertices-Temp";

    // Index buffer for static geometry
    const uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };
    sg_buffer_desc sokol_buffer_index { };
        sokol_buffer_index.type = SG_BUFFERTYPE_INDEXBUFFER;
        sokol_buffer_index.data = SG_RANGE(indices);
        sokol_buffer_index.label = "Indices-Temp";

    // Empty, dynamic instance-data vertex buffer (goes into vertex buffer bind slot 1)
    sg_buffer_desc sokol_buffer_instance_uv { };
        sokol_buffer_instance_uv.size = INSTANCES * sizeof(hmm_vec4);
        sokol_buffer_instance_uv.usage = SG_USAGE_STREAM;

    // Empty, dynamic instance-data vertex buffer (goes into vertex buffer bind slot 2)
    sg_buffer_desc sokol_buffer_instance_m { };
        sokol_buffer_instance_m.size = INSTANCES * sizeof(hmm_mat4);
        sokol_buffer_instance_m.usage = SG_USAGE_STREAM;

    // Bind Buffers
    bindings.vertex_buffers[0] =    sg_make_buffer(&sokol_buffer_vertex);
    bindings.vertex_buffers[1] =    sg_make_buffer(&sokol_buffer_instance_m);
    bindings.vertex_buffers[2] =    sg_make_buffer(&sokol_buffer_instance_uv);
    bindings.index_buffer =         sg_make_buffer(&sokol_buffer_index);

    // ***** Pipeline State Object
    sg_pipeline_desc (sokol_pipleine) { };
        sokol_pipleine.layout.buffers[0].stride = 44; //(sizeof(hmm_vec3) * (size_t)3) + ((sizeof(hmm_vec2) * (size_t)1));
        sokol_pipleine.layout.buffers[0].step_func = SG_VERTEXSTEP_PER_VERTEX;

        sokol_pipleine.layout.buffers[1].stride = 64; //(sizeof(hmm_mat4));
        sokol_pipleine.layout.buffers[1].step_func = SG_VERTEXSTEP_PER_INSTANCE;

        sokol_pipleine.layout.buffers[2].stride = 16; //(sizeof(hmm_vec4));
        sokol_pipleine.layout.buffers[2].step_func = SG_VERTEXSTEP_PER_INSTANCE;

        sokol_pipleine.layout.attrs[ATTR_vs_pos].format =           SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.layout.attrs[ATTR_vs_norm].format =          SG_VERTEXFORMAT_FLOAT3;
        sokol_pipleine.layout.attrs[ATTR_vs_texcoord0].format =     SG_VERTEXFORMAT_FLOAT2; //SG_VERTEXFORMAT_SHORT2N;
        sokol_pipleine.layout.attrs[ATTR_vs_bary].format =          SG_VERTEXFORMAT_FLOAT3;

        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat0].format = SG_VERTEXFORMAT_FLOAT4;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat1].format = SG_VERTEXFORMAT_FLOAT4;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat2].format = SG_VERTEXFORMAT_FLOAT4;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat3].format = SG_VERTEXFORMAT_FLOAT4;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_uv].format =   SG_VERTEXFORMAT_FLOAT4;

        sokol_pipleine.layout.attrs[ATTR_vs_pos].buffer_index =             0;
        sokol_pipleine.layout.attrs[ATTR_vs_norm].buffer_index =            0;
        sokol_pipleine.layout.attrs[ATTR_vs_texcoord0].buffer_index =       0;
        sokol_pipleine.layout.attrs[ATTR_vs_bary].buffer_index =            0;

        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat0].buffer_index =   1;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat1].buffer_index =   1;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat2].buffer_index =   1;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat3].buffer_index =   1;

        sokol_pipleine.layout.attrs[ATTR_vs_instance_uv].buffer_index =     2;

        sokol_pipleine.layout.attrs[ATTR_vs_pos].offset =           0;
        sokol_pipleine.layout.attrs[ATTR_vs_norm].offset =          12;
        sokol_pipleine.layout.attrs[ATTR_vs_texcoord0].offset =     24;
        sokol_pipleine.layout.attrs[ATTR_vs_bary].offset =          32;

        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat0].offset = 0;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat1].offset = 16;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat2].offset = 32;
        sokol_pipleine.layout.attrs[ATTR_vs_instance_mat3].offset = 48;

        sokol_pipleine.layout.attrs[ATTR_vs_instance_uv].offset =   0;

        sokol_pipleine.label = "Pipeline-BasicShader";
        sokol_pipleine.shader = sg_make_shader(basic_shader_shader_desc(sg_query_backend()));
        sokol_pipleine.colors[0].blend = sokol_blend_premultipied_alpha;

        sokol_pipleine.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
        //sokol_pipleine.index_type =   SG_INDEXTYPE_NONE;
        sokol_pipleine.index_type =     SG_INDEXTYPE_UINT16;
        //sokol_pipleine.cull_mode =    SG_CULLMODE_NONE;
        sokol_pipleine.cull_mode =      SG_CULLMODE_FRONT;
        sokol_pipleine.depth.compare =  SG_COMPAREFUNC_LESS_EQUAL;
        sokol_pipleine.depth.write_enabled = true;

    pipeline = sg_make_pipeline(&sokol_pipleine);

}

