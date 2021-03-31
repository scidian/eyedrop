//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#include "3rd_party/stb/stb_rect_pack.h"
#include "engine/app/image/Image.h"
#include "engine/app/image/Bitmap.h"
#include "engine/app/App.h"
#include "ImageManager.h"


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrImageManager::DrImageManager(int key_start) : DrKeys(key_start) {
    addAtlas(ATLAS_TYPE_ENGINE);
    addAtlas(ATLAS_TYPE_PROJECT);
}


//####################################################################################
//##    Sokol Helper
//####################################################################################
void DrImageManager::initializeSgImageDesc(const int& width, const int& height, sg_image_desc& image_desc) {
    image_desc.width =        width;
    image_desc.height =       height;
    image_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    image_desc.wrap_u =       SG_WRAP_CLAMP_TO_EDGE;        // also available: SG_WRAP_MIRRORED_REPEAT
    image_desc.wrap_v =       SG_WRAP_CLAMP_TO_EDGE;        // also available: SG_WRAP_MIRRORED_REPEAT
    image_desc.min_filter =   SG_FILTER_LINEAR;
    image_desc.mag_filter =   SG_FILTER_LINEAR;
}


//####################################################################################
//##    Image Functions
//####################################################################################
// Inits atlas on GPU, adds a into the Image Manager
void DrImageManager::addAtlas(Atlas_Type atlas_type) {
    // Check valid type
    if (atlas_type == ATLAS_TYPE_NONE) return;
    
    // Alloc an image on the gpu
    int gpu_handle = sg_alloc_image().id;

    // Create empty atlas
    std::shared_ptr<DrAtlas> atlas = std::make_shared<DrAtlas>();
        atlas->type =       atlas_type;
        atlas->id =         getNextKey();
        atlas->gpu =        gpu_handle;
        atlas->bitmap =     std::make_shared<DrBitmap>(MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, DROP_BITMAP_FORMAT_ARGB);
        atlas->rect_pack =  std::make_shared<stbrp_context>();
    
    // Add to atlases
    m_atlases.push_back(atlas);

    // Update image on gpu with new empty bitmap data
    sg_image_desc image_desc { };
        initializeSgImageDesc(atlas->bitmap->width, atlas->bitmap->height, image_desc);
        image_desc.data.subimage[0][0].ptr = &atlas->bitmap->data[0];
        image_desc.data.subimage[0][0].size = static_cast<size_t>(atlas->bitmap->width * atlas->bitmap->height * atlas->bitmap->channels);
    sg_init_image({static_cast<uint32_t>(atlas->gpu)}, &image_desc);
}


//####################################################################################
//##    Image Fetching
//####################################################################################
// Adds image to stack of images to be loaded
void DrImageManager::addImageToFetch(ImageData image_data) {
    m_load_image_stack.push_back(image_data);
}

// Initiates fetch of next image on the load stack
void DrImageManager::fetchNextImage() {
    if (m_loading_image || m_load_image_stack.size() < 1) return;
    m_loading_image = true;
    
    bool already_handled_fetch = false;
    #if defined(DROP_TARGET_HTML5)
        if (m_load_image_stack[0].was_dropped == true) {
            sapp_html5_fetch_request sokol_fetch_request { };
                sokol_fetch_request.dropped_file_index = 0;
                sokol_fetch_request.buffer_ptr = m_load_image_buffer;
                sokol_fetch_request.buffer_size = sizeof(m_load_image_buffer);
                sokol_fetch_request.callback = +[](const sapp_html5_fetch_response* response) {
                    // Load Data from response
                    DrBitmap bmp((unsigned char*)response->buffer_ptr, (int)response->fetched_size);
                    assert((bmp.width <= MAX_TEXTURE_SIZE && bmp.height <= MAX_TEXTURE_SIZE) && "Image dimensions too large! Max width and height are MAX_TEXTURE_SIZE!");
                    
                    // Could check for errors...
                    if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL     /* '1' */) { }

                    // Attempt to create image
                    g_app->imageManager()->createImage(bmp);
                };
            sapp_html5_fetch_dropped_file(&sokol_fetch_request);
            already_handled_fetch = true;
        }
    #endif
    
    if (already_handled_fetch == false) {
        sfetch_request_t sokol_fetch_image { };
            sokol_fetch_image.path = m_load_image_stack[0].image_file.c_str();
            sokol_fetch_image.buffer_ptr = m_load_image_buffer;
            sokol_fetch_image.buffer_size = sizeof(m_load_image_buffer);
            sokol_fetch_image.callback = +[](const sfetch_response_t* response) {
                // Load Data from response
                DrBitmap bmp((unsigned char*)response->buffer_ptr, (int)response->fetched_size);
                assert((bmp.width <= MAX_TEXTURE_SIZE && bmp.height <= MAX_TEXTURE_SIZE) && "Image dimensions too large! Max width and height are MAX_TEXTURE_SIZE!");

                // Could check for errors...
                if (response->error_code == SFETCH_ERROR_FILE_NOT_FOUND     /* '1' */) { }
                if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL   /* '3' */) { }

                // Attempt to create image
                g_app->imageManager()->createImage(bmp);
            };
        sfetch_send(&sokol_fetch_image);
    }
}

// Creates DrImage from DrBitmap from top of image loading stack, calls image callback function if there is one and image creation was successful
void DrImageManager::createImage(DrBitmap& bmp) {
    if (bmp.isValid()) {
        sg_image_desc image_desc { };
            initializeSgImageDesc(bmp.width, bmp.height, image_desc);
            image_desc.data.subimage[0][0].ptr = &bmp.data[0];
            image_desc.data.subimage[0][0].size = static_cast<size_t>(bmp.width * bmp.height * bmp.channels);
        m_load_image_stack[0].image = std::make_shared<DrImage>(m_load_image_stack[0].image_file, bmp, m_load_image_stack[0].outline);
        m_load_image_stack[0].image->setID(sg_make_image(&image_desc).id);

        // Call callback function
        if (m_load_image_stack[0].callback != NULL) {
            m_load_image_stack[0].callback(m_load_image_stack[0].image);
        }
    }

    // Remove image of list to be fetched
    m_load_image_stack.pop_front();
    m_loading_image = false;
}



