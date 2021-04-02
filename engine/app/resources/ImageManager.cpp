//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#include "3rd_party/stb/stb_rect_pack.h"
#include "engine/app/core/Math.h"
#include "engine/app/geometry/Point.h"
#include "engine/app/geometry/Rect.h"
#include "engine/app/image/Image.h"
#include "engine/app/image/Bitmap.h"
#include "engine/app/App.h"
#include "ImageManager.h"


//####################################################################################
//##    Constants
//####################################################################################
#define NODE_COUNT  (MAX_TEXTURE_SIZE * 2)


//####################################################################################
//##    Sokol Helper
//####################################################################################
// Sokol image description initiliazer
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
//##    Atlas Struct
//####################################################################################
struct DrAtlas {
    Atlas_Type                      type;                                           // Type of this Atlas
    int                             key                     { KEY_NONE };           // Image Manager unique indentifier
    std::shared_ptr<DrBitmap>       bitmap                  { nullptr };            // Atlas in system memory
    int                             gpu                     { INVALID_IMAGE };      // Atlas in gpu memory
    std::shared_ptr<stbrp_context>  rect_pack               { nullptr };            // Stb Rect Pack Context
    std::vector<int>                packed_image_keys       { };                    // Images packed onto this atlas
    std::vector<stbrp_node>         nodes;
};


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrImageManager::DrImageManager(std::vector<int> key_starts) : DrKeys(IMAGE_KEY_TOTAL, key_starts) {
    addAtlas(ATLAS_TYPE_ENGINE,     MAX_TEXTURE_SIZE);
    addAtlas(ATLAS_TYPE_PROJECT,    MAX_TEXTURE_SIZE);
}


//####################################################################################
//##    Atlas Creation
//####################################################################################
// Inits Atlas on GPU, adds a into the Image Manager
std::shared_ptr<DrAtlas>& DrImageManager::addAtlas(Atlas_Type atlas_type, int atlas_size) {
    // Create empty atlas
    std::shared_ptr<DrAtlas> atlas = std::make_shared<DrAtlas>();
        atlas->type = atlas_type;
        atlas->key = getNextKey(IMAGE_KEY_ATLAS);
        atlas->gpu = sg_alloc_image().id;                                           // Alloc an image on the gpu  
        atlas->bitmap = std::make_shared<DrBitmap>(atlas_size, atlas_size, DROP_BITMAP_FORMAT_ARGB);
    

    // No rect pack for single images
    if (atlas_type == ATLAS_TYPE_NONE) {   
        atlas->rect_pack =  nullptr;
        atlas->nodes.resize(0);
    
    // Rect pack memory for multi image atlases
    } else {
        atlas->rect_pack =  std::make_shared<stbrp_context>();
        atlas->nodes.resize(NODE_COUNT);
        
        // Initialize Rect Pack
        stbrp_init_target(atlas->rect_pack.get(), atlas_size, atlas_size, &atlas->nodes[0], NODE_COUNT);
    }

    // Update image on gpu with new empty bitmap data
    sg_image_desc image_desc { };
        initializeSgImageDesc(atlas->bitmap->width, atlas->bitmap->height, image_desc);
        image_desc.data.subimage[0][0].ptr = &atlas->bitmap->data[0];
        image_desc.data.subimage[0][0].size = static_cast<size_t>(atlas->bitmap->width * atlas->bitmap->height * atlas->bitmap->channels);
    sg_init_image({static_cast<uint32_t>(atlas->gpu)}, &image_desc);

    // Add to atlases
    if (atlas_type == ATLAS_TYPE_NONE) {
        m_atlas_single.push_back(atlas);
        return m_atlas_single.back();
    } else {
        m_atlas_multi.push_back(atlas);
        return m_atlas_multi.back();
    }    
}


//####################################################################################
//##    Atlas Packing
//####################################################################################
// Packs new Image onto an available Atlas
void DrImageManager::findAtlasForImage(ImageData& image_data) {
    // Loop through available atlases, attempt to pack
    if (image_data.atlas_type != ATLAS_TYPE_NONE) {
        for (auto atlas : m_atlas_multi) {
            if (atlas->type != image_data.atlas_type) continue;                     // Wrong atlas type, skip
            if (addImageToAtlas(image_data, atlas)) return;                         // Attempt to add to atlas
        }
    }

    // If we're still here we need a new Atlas, Image didn't fit in current Atlases!!
    int max_dimension = image_data.image->bitmap().maxDimension();
    int atlas_size = (image_data.atlas_type == ATLAS_TYPE_NONE) ? RoundPowerOf2(max_dimension) : (MAX_TEXTURE_SIZE);
    std::shared_ptr<DrAtlas> atlas = addAtlas(image_data.atlas_type, atlas_size);
    addImageToAtlas(image_data, atlas);
}

//############################

// Fills a Stb Rect
void SetStbRect(stbrp_rect& rect, std::shared_ptr<DrImage>& image) {
    rect.id =   image->key();
    rect.w =    image->bitmap().width;
    rect.h =    image->bitmap().height;
    rect.x =    0;      
    rect.y =    0;      
    rect.was_packed = 0;
}

// Attempts to put an Image on an Atlas, returns true if successful
bool DrImageManager::addImageToAtlas(ImageData& image_data, std::shared_ptr<DrAtlas>& atlas) {
    // ***** Test if new image fits
    std::vector<stbrp_rect> new_image_rect(1);
    SetStbRect(new_image_rect[0], image_data.image);
    stbrp_pack_rects(atlas->rect_pack.get(), &new_image_rect[0], 1);

    //  Image didn't fit, return false
    if (new_image_rect[0].was_packed == false) return false;
        
    // ***** Image fits, repack atlas nicely *****

    // Add new image key to list of packed images
    atlas->packed_image_keys.push_back(new_image_rect[0].id);

    // Fill rects with image data
    size_t num_rects = atlas->packed_image_keys.size();
    std::vector<stbrp_rect> rects(num_rects);
    for (int i = 0; i < num_rects; ++i) {
        SetStbRect(rects[i], m_images[atlas->packed_image_keys[i]]);
    }

    // Reset stb rect pack context, pack rects
    stbrp_init_target(atlas->rect_pack.get(), MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, &atlas->nodes[0], NODE_COUNT);
    stbrp_pack_rects(atlas->rect_pack.get(), &rects[0], num_rects);

    // Copy image pixel data to atlas
    atlas->bitmap->clearPixels();
    for (int i = 0; i < atlas->packed_image_keys.size(); ++i) {
        DrRect source_rect = m_images[rects[i].id]->bitmap().rect();
        DrPoint dest_point = DrPoint(rects[i].x, rects[i].y);
        DrBitmap::Blit(m_images[rects[i].id]->bitmap(), source_rect, *(atlas->bitmap.get()), dest_point);


        // !!!!!!!!!!!! NEED TO UPDATE DrImage.uv0 AND DrImage.uv1 COORDINATES HERE NOW !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    }

    // Update image on gpu with new bitmap data
    sg_uninit_image({static_cast<uint32_t>(atlas->gpu)});
    sg_image_desc image_desc { };
        initializeSgImageDesc(atlas->bitmap->width, atlas->bitmap->height, image_desc);
        image_desc.data.subimage[0][0].ptr = &atlas->bitmap->data[0];
        image_desc.data.subimage[0][0].size = static_cast<size_t>(atlas->bitmap->width * atlas->bitmap->height * atlas->bitmap->channels);
    sg_init_image({static_cast<uint32_t>(atlas->gpu)}, &image_desc);

    return true;
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
                    App()->imageManager()->createImage(bmp);
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
                App()->imageManager()->createImage(bmp);
            };
        sfetch_send(&sokol_fetch_image);
    }
}


//####################################################################################
//##    Image Creation
//####################################################################################
// Creates DrImage from DrBitmap from top of image loading stack, calls image callback function if there is one and image creation was successful
void DrImageManager::createImage(DrBitmap& bmp) {
    if (bmp.isValid()) {
        // Image data
        ImageData& image_data = m_load_image_stack[0];
        int new_image_key = getNextKey(IMAGE_KEY_IMAGE);

        // Load onto GPU
        sg_image_desc image_desc { };
            initializeSgImageDesc(bmp.width, bmp.height, image_desc);
            image_desc.data.subimage[0][0].ptr = &bmp.data[0];
            image_desc.data.subimage[0][0].size = static_cast<size_t>(bmp.width * bmp.height * bmp.channels);
        uint32_t gpu_id = sg_make_image(&image_desc).id;

        // Create DrImage
        image_data.image = std::make_shared<DrImage>(image_data.image_file, bmp, image_data.outline);
        image_data.image->setKey(new_image_key);
        image_data.image->setGpuID(gpu_id);

        // Save copy of pointer to Image Manager
        m_images[new_image_key] = image_data.image;

        // If there is a callback function, call it now
        if (image_data.callback != NULL) {
            image_data.callback(image_data.image);
        }
    
        // Pack Image onto an Atlas
        findAtlasForImage(image_data);
    }

    // Remove image of list to be fetched
    m_load_image_stack.pop_front();
    m_loading_image = false;
}



