//
// Copyright (C) 2021 Scididan Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
//####################################################################################
//##    Includes
//####################################################################################
#include "../3rd_party/handmade_math.h"
#include "../3rd_party/stb/stb_image.h"
#include "../core/compare.h"
#include "../core/imaging.h"
#include "../core/random.h"
#include "../core/types/bitmap.h"
#include "../core/types/color.h"
#include "../core/types/image.h"
#include "../core/types/rect.h"
#include "../core/types/vec2.h"
#include "../engine/s3d/mesh.h"

// ***** Sokol
#include "../3rd_party/sokol/sokol_app.h"
#include "../3rd_party/sokol/sokol_gfx.h"
#include "../3rd_party/sokol/sokol_gl.h"
#include "../3rd_party/sokol/sokol_glue.h"
#include "../3rd_party/sokol/sokol_time.h"
#include "../3rd_party/sokol/sokol_audio.h"
#include "../3rd_party/sokol/sokol_fetch.h"

// ***** Fonts
#include "../3rd_party/fontstash.h"
#include "../3rd_party/sokol/sokol_fontstash.h"

// ***** ImGui
#include "../3rd_party/imgui/imgui.h"
#include "../3rd_party/imgui_font.h"
#define SOKOL_IMGUI_IMPL
#include "../3rd_party/sokol/sokol_imgui.h"

// ***** Debug Menu
#ifdef DEBUG_ON
    #define SOKOL_GFX_IMGUI_IMPL
    #include "../3rd_party/sokol/sokol_gfx_imgui.h"
#endif

// ***** Html5 / File Handling
#ifndef __EMSCRIPTEN__
    #include "../3rd_party/wai/whereami.c"
#else
    #include <emscripten/emscripten.h>
    #include <emscripten/html5.h>
#endif

// ***** Local Includes
#include "input.h"
#include "../shaders/basic_shader.glsl.h"


//####################################################################################
//##    Local Structs / Defines
//####################################################################################
#define MAX_FILE_SIZE (1024 * 1024)

enum loadstate_t {
    LOADSTATE_UNKNOWN = 0,
    LOADSTATE_SUCCESS,
    LOADSTATE_FAILED,
    LOADSTATE_FILE_TOO_BIG,
};

struct item_t {
    sapp_event event = { };
};

struct state_t {
    // Gfx
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;

    // Fetch / Drop
    uint8_t file_buffer[MAX_FILE_SIZE];
    loadstate_t load_state;

    // Events
    item_t items[_SAPP_EVENTTYPE_NUM];

    // Font
    FONScontext* fons;
    float   dpi_scale;
    int     font_normal;
    uint8_t font_normal_data[MAX_FILE_SIZE];
};



//####################################################################################
//##    Globals
//####################################################################################
// Sokol Variables
sapp_desc sokol_app;
static state_t state;
#ifdef DEBUG_ON
    static sg_imgui_t sg_imgui;
#endif

// Image Variables
std::shared_ptr<DrMesh> mesh = std::make_shared<DrMesh>();
std::shared_ptr<DrImage> image = nullptr;
bool initialized_image = false;
int  mesh_quality = 5;
int  before_keys = mesh_quality;

// FPS Variables
uint64_t time_start = 0;
long ticks = 0;
long fps =   0;

// Model Rotation
DrVec2      total_rotation {  0.f,  0.f };
DrVec2      add_rotation   { 25.f, 25.f };
hmm_mat4    model =         Dr::IdentityMatrix();
DrVec2      mouse_down =    { 0, 0 };
float       rotate_speed =  1.f;
bool        is_mouse_down = false;
float       zoom = 1.5f;
bool        wireframe = true;


//####################################################################################
//##    Sokol-fetch load callbacks 
//####################################################################################
static void image_loaded(const sfetch_response_t*);
static void font_normal_loaded(const sfetch_response_t* response) {
    if (response->fetched) {
        state.font_normal = fonsAddFontMem(state.fons, "sans", (unsigned char*)response->buffer_ptr, (int)response->fetched_size,  false);
    } 
}


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
//##    Initialize
//####################################################################################
void init(void) {
    // ***** Setup sokol-gfx, call sokol_glue function to obtain values from sokol_app
    sg_desc (sokol_gfx) {
        .context = sapp_sgcontext()
    };            
    sg_setup(&sokol_gfx);

    // ***** Setup sokol-gl
    sgl_desc_t (sokol_gl) { 0 };
    sgl_setup(&sokol_gl);

    // ***** Setup sokol-time
    stm_setup();
    time_start = stm_now();

    // ***** Setup sokol-fetch (for loading files) with the minimal "resource limits"
    sfetch_desc_t (sokol_fetch) {
        .max_requests = 4,
        .num_channels = 2,
        .num_lanes = 2
    };
    sfetch_setup(&sokol_fetch);

    // ***** Font Setup, make sure the fontstash atlas width/height is pow-2 
    state.dpi_scale = sapp_dpi_scale();
    const int atlas_dim = Dr::RoundPowerOf2(512.0f * state.dpi_scale);
    FONScontext* fons_context = sfons_create(atlas_dim, atlas_dim, FONS_ZERO_TOPLEFT);
    state.fons = fons_context;
    state.font_normal = FONS_INVALID;          


    // ***** Setup debug inspection header(s)
    #ifdef DEBUG_ON
        sg_imgui_init(&sg_imgui);
    #endif


    // ***** Setup sokol-imgui, provide our own font
    simgui_desc_t (simgui_desc) = {
        .sample_count = sapp_sample_count(),
        .no_default_font = true,
        .dpi_scale = sapp_dpi_scale(),
    };
    simgui_setup(&simgui_desc);

    // Configure Dear ImGui with our own embedded font
    auto& io = ImGui::GetIO();
    ImFontConfig fontCfg {};
        fontCfg.FontDataOwnedByAtlas = false;
        fontCfg.OversampleH = 2;
        fontCfg.OversampleV = 2;
        fontCfg.RasterizerMultiply = 1.5f;
    io.Fonts->AddFontFromMemoryTTF(dump_font, sizeof(dump_font), 16.0f, &fontCfg);

    // Framed widgets
    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding =       4.0f;
    style.FrameBorderSize =     1.0f;
    style.WindowBorderSize =    1.0f;

    // Create font texture for the custom font
    unsigned char* font_pixels;
    int font_width, font_height;
    io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);
    sg_image_desc img_desc = {
        .width = font_width,
        .height = font_height,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
        .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
        .min_filter = SG_FILTER_LINEAR,
        .mag_filter = SG_FILTER_LINEAR,
        .data.subimage[0][0].ptr = font_pixels,
        .data.subimage[0][0].size = static_cast<size_t>(font_width * font_height * 4),
    };
    io.Fonts->TexID = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
   


    // ***** Pass action for clearing the framebuffer to some color
    state.pass_action.colors[0].action = SG_ACTION_CLEAR;
    state.pass_action.colors[0].value = { 0.125f, 0.25f, 0.35f, 1.0f };


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
    state.bind.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

    // Index buffer
    const uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };
    sg_buffer_desc (sokol_buffer_index) {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "temp-indices"
    };
    state.bind.index_buffer = sg_make_buffer(&(sokol_buffer_index));


    // ***** Pipeline State Object, sets 3D device parameters
    sg_pipeline_desc (sokol_pipleine) {
        .shader = sg_make_shader(extrude3D_shader_desc(sg_query_backend())),
        .layout = {
            .attrs = {
                [ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_vs_norm].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_vs_texcoord0].format = SG_VERTEXFORMAT_FLOAT2, //SG_VERTEXFORMAT_SHORT2N,
                [ATTR_vs_bary].format = SG_VERTEXFORMAT_FLOAT3,
            }
        },
        //.primitive_type  = SG_PRIMITIVETYPE_TRIANGLES,
        //.index_type = SG_INDEXTYPE_NONE,
        .index_type = SG_INDEXTYPE_UINT16,
        //.cull_mode = SG_CULLMODE_NONE, 
        .cull_mode = SG_CULLMODE_FRONT,
        .depth = {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true
        },
        .label = "extrude-pipeline",
        .colors[0].blend = sokol_blend_alpha,
    };
    state.pip = sg_make_pipeline(&sokol_pipleine);


    // ***** Allocate an image handle, 
    //  but don't actually initialize the image yet, this happens later when the asynchronous file load has finished.
    //  Any draw calls containing such an "incomplete" image handle will be silently dropped.
    state.bind.fs_images[SLOT_tex] = sg_alloc_image();

    // ***** Start loading the PNG File
    //  We don't need the returned handle since we can also get that inside the fetch-callback from the response
    //  structure. NOTE: we're not using the user_data member, since all required state is in a global variable anyway
    char* path = NULL;
    int length, dirname_length;
    std::string image_file = "", font_file = "";

    #ifndef __EMSCRIPTEN__
        length = wai_getExecutablePath(NULL, 0, &dirname_length);
        if (length > 0) {
            path = (char*)malloc(length + 1);
            wai_getExecutablePath(path, length, &dirname_length);
            //path[length] = '\0';
            //printf("executable path: %s\n", path);
            path[dirname_length] = '\0';
            //printf("  dirname: %s\n", path);
            //printf("  basename: %s\n", path + dirname_length + 1);
            std::string base = path;
            image_file = base + "/assets/blob.png";
            font_file  = base + "/assets/aileron-regular.otf";
            free(path);
        }
    #else        
        // ********** NOTE: About loading images with Emscripten **********
        //  When running html on local machine, must disable CORS in broswer
        //  On Safari, with 'Develop' menu enabled select "Disable Cross-Origin Restrictions"
        //image_file = "http://github.com/stevinz/extrude/blob/master/assets/shapes.png?raw=true";
        image_file = "assets/shapes.png";
        font_file  = "assets/aileron-regular.otf";
    #endif


    // Load inital "shapes.png" image in background
    sfetch_request_t (sokol_fetch_image) {
        .path = image_file.c_str(),
        .callback = image_loaded,
        .buffer_ptr = state.file_buffer,
        .buffer_size = sizeof(state.file_buffer)
    };
    sfetch_send(&sokol_fetch_image);

    // Load font in background
    sfetch_request_t (sokol_fetch_font) {
        .path = font_file.c_str(),
        .callback = font_normal_loaded,
        .buffer_ptr = state.font_normal_data,
        .buffer_size = sizeof(state.font_normal_data)
    };
    sfetch_send(&sokol_fetch_font);
}



//####################################################################################
//##    Create 3D extrusion
//####################################################################################
void calculateMesh(bool reset_position) {
    //##    Level of Detail:
    //##        0.075 = Detailed
    //##        0.250 = Nice
    //##        1.000 = Low poly
    //##       10.000 = Really low poly
    float level_of_detail = 0.6f;
    switch (mesh_quality) {
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
    image->outlinePoints(level_of_detail);
    mesh = std::make_shared<DrMesh>();
    mesh->image_size = Dr::Max(image->getBitmap().width, image->getBitmap().height);      
    mesh->wireframe = wireframe;
    mesh->initializeExtrudedImage(image.get(), mesh_quality);
    //mesh->initializeTextureQuad();
    //mesh->initializeTextureCube();
    //mesh->initializeTextureCone();
             
    // ***** Copy vertex data and set into state buffer
    if (mesh->vertexCount() > 0) {
        // ***** Vertex Buffer
        unsigned int total_vertices = mesh->vertices.size();

        Vertex vertices[total_vertices];
        for (size_t i = 0; i < total_vertices; i++) vertices[i] = mesh->vertices[i];
        sg_buffer_desc (sokol_buffer_vertex) {
            .data = SG_RANGE(vertices),
            .label = "extruded-vertices"
        };
        sg_destroy_buffer(state.bind.vertex_buffers[0]);
        state.bind.vertex_buffers[0] = sg_make_buffer(&sokol_buffer_vertex);

        // ***** Index Buffer
        unsigned int total_indices = mesh->indices.size();
        uint16_t indices[total_indices];
        for (size_t i = 0; i < total_indices; i++) indices[i] = mesh->indices[i];
        sg_buffer_desc (sokol_buffer_index) {
            .type = SG_BUFFERTYPE_INDEXBUFFER,
            .data = SG_RANGE(indices),
            .label = "temp-indices"
        };
        sg_destroy_buffer(state.bind.index_buffer);
        state.bind.index_buffer = sg_make_buffer(&(sokol_buffer_index));

        // ***** Reset rotation
        if (reset_position) {
            total_rotation.set(0.f, 0.f);
            add_rotation.set(25.f, 25.f);
            model = Dr::IdentityMatrix();
        }
    }
}

//####################################################################################
//##    Load Image
//####################################################################################
static void load_image(stbi_uc *buffer_ptr, int fetched_size) {
    int png_width, png_height, num_channels;
    const int desired_channels = 4;
    stbi_uc* pixels = stbi_load_from_memory(buffer_ptr, fetched_size, &png_width, &png_height, &num_channels, desired_channels);

    // Stb Load Succeeded
    if (pixels) {

        // ********** Copy data into our custom bitmap class, create image and trace outline
        DrBitmap bitmap = DrBitmap(pixels, static_cast<int>(png_width * png_height * 4), false, png_width, png_height);
        //bitmap = Dr::ApplySinglePixelFilter(Image_Filter_Type::Hue, bitmap, Dr::RandomInt(-100, 100));
        image = std::make_shared<DrImage>("shapes", bitmap, 0.25f);

        calculateMesh(true);        

        // ********** Initialze the sokol-gfx texture
        sg_image_desc (sokol_image) {
            .width =  image->getBitmap().width,
            .height = image->getBitmap().height,
            .pixel_format = SG_PIXELFORMAT_RGBA8,
            .wrap_u = SG_WRAP_MIRRORED_REPEAT,
            .wrap_v = SG_WRAP_MIRRORED_REPEAT,
            .min_filter = SG_FILTER_LINEAR,
            .mag_filter = SG_FILTER_LINEAR,
            .data.subimage[0][0] = {
                .ptr =  &(image->getBitmap().data[0]),
                .size = (size_t)image->getBitmap().size(),
            }
        };
        
        // If we already have an image in the state buffer, uninit before initializing new image
        if (initialized_image == true) { sg_uninit_image(state.bind.fs_images[SLOT_tex]); }

        // Initialize new image into state buffer
        sg_init_image(state.bind.fs_images[SLOT_tex], &sokol_image);
        initialized_image = true;
        stbi_image_free(pixels);
    }
}

//####################################################################################
//##    Callback: File Loading
//####################################################################################
/* The fetch-callback is called by sokol_fetch.h when the data is loaded,
   or when an error has occurred. */
static void image_loaded(const sfetch_response_t* response) {
    if (response->fetched) {
        // File data has been fetched
        //  Since we provided a big-enough buffer we can be sure that all data has been loaded here
        load_image((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
    }
    else if (response->finished) {
        // If loading the file failed, set clear color to signal reason
        if (response->failed) {
            sg_pass_action (pass_action0) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 1.0f, 1.0f, 1.0f } } };        // white
            sg_pass_action (pass_action1) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 0.0f, 0.0f, 1.0f } } };        // red
            sg_pass_action (pass_action2) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.0f, 1.0f, 0.0f, 1.0f } } };        // green
            sg_pass_action (pass_action3) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.0f, 0.0f, 1.0f, 1.0f } } };        // blue 
            sg_pass_action (pass_action4) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 1.0f, 0.0f, 1.0f } } };        // yellow
            sg_pass_action (pass_action5) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.0f, 1.0f, 1.0f, 1.0f } } };        // cyan
            sg_pass_action (pass_action6) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 0.0f, 1.0f, 1.0f } } };        // magenta
            sg_pass_action (pass_action7) { .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.5f, 0.5f, 0.5f, 1.0f } } };        // black
    
            switch (response->error_code) {
                case SFETCH_ERROR_NO_ERROR:             state.pass_action = (pass_action0);     break;
                case SFETCH_ERROR_FILE_NOT_FOUND:       state.pass_action = (pass_action1);     break;
                case SFETCH_ERROR_NO_BUFFER:            state.pass_action = (pass_action2);     break;
                case SFETCH_ERROR_BUFFER_TOO_SMALL:     state.pass_action = (pass_action3);     break;
                case SFETCH_ERROR_UNEXPECTED_EOF:       state.pass_action = (pass_action4);     break;
                case SFETCH_ERROR_CANCELLED:            state.pass_action = (pass_action5);     break;
                case SFETCH_ERROR_INVALID_HTTP_STATUS:  state.pass_action = (pass_action6);     break;
                default:                                state.pass_action = (pass_action7);
            }            
        }
    }
}


//####################################################################################
//##    Callback: File Dropped
//####################################################################################
#if defined(__EMSCRIPTEN__)
// the async-loading callback for sapp_html5_fetch_dropped_file
static void emsc_load_callback(const sapp_html5_fetch_response* response) {
    if (response->succeeded) {
        state.load_state = LOADSTATE_SUCCESS;
        load_image((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
    } else if (SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL == response->error_code) {
        state.load_state = LOADSTATE_FILE_TOO_BIG;
    } else {
        state.load_state = LOADSTATE_FAILED;
    }
}
#else
// the async-loading callback for native platforms
static void native_load_callback(const sfetch_response_t* response) {
    if (response->fetched) {
        state.load_state = LOADSTATE_SUCCESS;
        load_image((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
    } else if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL) {
        state.load_state = LOADSTATE_FILE_TOO_BIG;
    } else {
        state.load_state = LOADSTATE_FAILED;
    }
}
#endif


//####################################################################################
//##    Input
//####################################################################################
static void input(const sapp_event* event) {
    state.items[event->type].event = *event;

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
                mesh_quality = event->key_code - SAPP_KEYCODE_1;
                break;
            case SAPP_KEYCODE_R:
                total_rotation.set(0.f, 0.f);
                add_rotation.set(25.f, 25.f);
                model = Dr::IdentityMatrix();
                break;
            case SAPP_KEYCODE_W:
                mesh->wireframe = !mesh->wireframe;
                wireframe = mesh->wireframe;
                break;
            default: ;
        }
                
    } else if (event->type == SAPP_EVENTTYPE_MOUSE_SCROLL) {
        zoom -= (event->scroll_y * 0.1f);
        zoom = Dr::Clamp(zoom, 0.5f, 5.0f);

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            mouse_down.set(event->mouse_y, event->mouse_x);
            is_mouse_down = true;
        }
        
    } else if (event->type == SAPP_EVENTTYPE_MOUSE_UP) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            is_mouse_down = false;
        }

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
        if (is_mouse_down) {
            float x_movement = event->mouse_y;
            float y_movement = event->mouse_x;

            if (mouse_down.x < x_movement) {
                add_rotation.x = rotate_speed * (x_movement - mouse_down.x);
            } else if (mouse_down.x > x_movement) {
                add_rotation.x = 360 - (rotate_speed * (mouse_down.x - x_movement));
            }
            
            if (mouse_down.y > y_movement) {
                add_rotation.y = 360 - (rotate_speed * (mouse_down.y - y_movement));
            } else if (mouse_down.y < y_movement) {
                add_rotation.y = rotate_speed * (y_movement - mouse_down.y);
            }

            mouse_down.x = x_movement;
            mouse_down.y = y_movement;
            add_rotation.x = Dr::EqualizeAngle0to360(add_rotation.x);
            add_rotation.y = Dr::EqualizeAngle0to360(add_rotation.y);
        }

    } else if (event->type == SAPP_EVENTTYPE_FILES_DROPPED) {
        #if defined(__EMSCRIPTEN__)
            // on emscripten need to use the sokol-app helper function to load the file data
            sapp_html5_fetch_request (sokol_fetch_request) {
                .dropped_file_index = 0,
                .callback = emsc_load_callback,
                .buffer_ptr = state.file_buffer,
                .buffer_size = sizeof(state.file_buffer),
            };
            sapp_html5_fetch_dropped_file(&sokol_fetch_request);
        #else
            // native platform: use sokol-fetch to load file content
            sfetch_request_t (sokol_fetch_request) {
                .path = sapp_get_dropped_file_path(0),
                .callback = native_load_callback,
                .buffer_ptr = state.file_buffer,
                .buffer_size = sizeof(state.file_buffer)
            };
            sfetch_send(&sokol_fetch_request);
        #endif
    }

    simgui_handle_event(event);
}



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
//##    Render
//####################################################################################
// Was playing with getting canvas size from html file to resize framebuffer when
// canvas is resized. Went around adding to sokol_app by invoking 'resize' event
// directly from javascript in the webpage on canvas resize.
#if defined(__EMSCRIPTEN__)
    EM_JS(int, get_canvas_width,  (), { return canvas.width; });
    EM_JS(int, get_canvas_height, (), { return canvas.height; });
#endif

/* The frame-function is fairly boring, note that no special handling is
   needed for the case where the texture isn't loaded yet.
   Also note the sfetch_dowork() function, this is usually called once a
   frame to pump the sokol-fetch message queues.
*/
static void frame(void) {
    // ***** Pump the sokol-fetch message queues, and invoke response callbacks
    sfetch_dowork();

    // ***** Compute model-view-projection matrix for vertex shader
    hmm_mat4 proj = HMM_Perspective(52.5f, (float)sapp_width()/(float)sapp_height(), 5.f, 20000.0f);
    hmm_mat4 view = HMM_LookAt(HMM_Vec3(0.0f, 1.5f, mesh->image_size * zoom), HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 view_proj = HMM_MultiplyMat4(proj, view);

    hmm_mat4 rxm = HMM_Rotate(add_rotation.x, HMM_Vec3(1.0f, 0.0f, 0.0f));
    hmm_mat4 rym = HMM_Rotate(add_rotation.y, HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 rotate = HMM_MultiplyMat4(rxm, rym); 
             model =  HMM_MultiplyMat4(rotate, model);
    total_rotation.x = Dr::EqualizeAngle0to360(total_rotation.x + add_rotation.x);
    total_rotation.y = Dr::EqualizeAngle0to360(total_rotation.y + add_rotation.y);
    add_rotation.set(0.f, 0.f);


    // Uniforms for vertex shader
    vs_params_t vs_params;
    vs_params.m =   model;
    vs_params.mvp = HMM_MultiplyMat4(view_proj, model);
    
    // Uniforms for fragment shader
    fs_params_t fs_params;
    fs_params.u_wireframe = (mesh->wireframe) ? 1.0f : 0.0f;


    // Check if user requested new model quality, if so recalculate
    if (mesh_quality != before_keys) {
        calculateMesh(false);
        before_keys = mesh_quality;
    }


    // ***** Render pass
    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    sg_draw(0, mesh->indices.size(), 1);


    // ***** Text
    fonsClearState(state.fons);    
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, sapp_widthf(), sapp_heightf(), 0.0f, -1.0f, +1.0f);

    const float dpis = state.dpi_scale;
    FONScontext* fs = state.fons;

    if (state.font_normal != FONS_INVALID) {
        fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_BASELINE);
        fonsSetFont(fs, state.font_normal);
        fonsSetSize(fs, 18.0f * dpis);
        fonsSetColor(fs, sfons_rgba(255, 255, 255, 255));
        fonsSetBlur(fs, 0);
        fonsSetSpacing(fs, 0.0f);
        fonsDrawText(fs, 10 * dpis, 20 * dpis, ("FPS: " +  std::to_string(fps)).c_str(), NULL);
        fonsDrawText(fs, 10 * dpis, 40 * dpis, ("Quality: " + std::to_string(mesh_quality+1)).c_str(), NULL);
        fonsDrawText(fs, 10 * dpis, 60 * dpis, ("Triangles: " + std::to_string(mesh->indexCount() / 3)).c_str(), NULL);
        //fonsDrawText(fs, 10 * dpis, 80 * dpis, ("ZOOM: " + std::to_string(zoom)).c_str(), NULL);
    }
    sfons_flush(fs);            // Flush fontstash's font atlas to sokol-gfx texture
    sgl_draw();


    // ***** GUI
    int width = sapp_width();
    int height = sapp_height();
    simgui_new_frame(width, height, 1.0/60.0);

    // Demo Window
    ImGui::SetNextWindowPos(ImVec2(40, 40), ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow();
    
    // Theme selector
    theme_generator();

    // Debug Sokol
    #ifdef DEBUG_ON
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("sokol-gfx")) {
                ImGui::MenuItem("Capabilities", 0, &sg_imgui.caps.open);
                ImGui::MenuItem("Buffers", 0, &sg_imgui.buffers.open);
                ImGui::MenuItem("Images", 0, &sg_imgui.images.open);
                ImGui::MenuItem("Shaders", 0, &sg_imgui.shaders.open);
                ImGui::MenuItem("Pipelines", 0, &sg_imgui.pipelines.open);
                ImGui::MenuItem("Passes", 0, &sg_imgui.passes.open);
                ImGui::MenuItem("Calls", 0, &sg_imgui.capture.open);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        sg_imgui_draw(&sg_imgui);
    #endif
    
    // Render ImGui
    simgui_render();
    
    

    // ***** End Rendering
    sg_end_pass();
    sg_commit();


    // ***** Update frames per second
    ticks++;
    uint64_t elapsed = stm_since(time_start);
    double seconds = stm_sec(elapsed);
    if (seconds >= 1.0) {
        fps = ticks;
        time_start = stm_now();
        ticks = 0;
    }
}

//####################################################################################
//##    Clean Up
//####################################################################################
void cleanup(void) {
    sfetch_shutdown();
    sfons_destroy(state.fons);
    sgl_shutdown();
    simgui_shutdown();
    #ifdef DEBUG_ON
        sg_imgui_discard(&sg_imgui);
    #endif
    sg_shutdown();
}

//####################################################################################
//##    App Entry
//####################################################################################
sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; 
    (void)argv;
    sokol_app.window_title = "ImGui Test";
    sokol_app.init_cb = init;
    sokol_app.frame_cb = frame;
    sokol_app.event_cb = input;
    sokol_app.cleanup_cb = cleanup;
    sokol_app.width = 800;
    sokol_app.height = 600;
    sokol_app.enable_clipboard = true;
    sokol_app.enable_dragndrop = true;
    sokol_app.max_dropped_files = 1;
    return sokol_app;
}






