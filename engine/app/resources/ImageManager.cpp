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
//##    Constructor / Destructor
//####################################################################################
DrImageManager::DrImageManager(int atlas_key_start, int image_key_start) : 
    m_atlas_keys(atlas_key_start),
    m_image_keys(image_key_start)
{ }


//####################################################################################
//##    Struct Initializers
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
// Stb rect initializer
void DrImageManager::setStbRect(stbrp_rect& rect, std::shared_ptr<DrImage>& image) {
    rect.id =   image->key();
    rect.w =    image->bitmap().width;
    rect.h =    image->bitmap().height;
    rect.x =    0;      
    rect.y =    0;      
    rect.was_packed = 0;
}


//####################################################################################
//##    Atlas Checking
//####################################################################################
std::shared_ptr<DrAtlas>& DrImageManager::atlasFromGpuID(int gpu_id) {
    for (auto& pair : m_atlas_multi) {
        if (pair.second->gpu == gpu_id) return pair.second;
    }
    for (auto& pair : m_atlas_single) {
        if (pair.second->gpu == gpu_id) return pair.second;
    }
    assert(false && "No atlas found with the requested gpu_id!!");
}


//####################################################################################
//##    Image Fetching
//####################################################################################
// Adds image to stack of images to be loaded
void DrImageManager::addImageToFetch(ImageLoadData image_data) {
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

                    // Image dimensions too large! Max width and height are MAX_IMAGE_SIZE!"
                    if (bmp.width > MAX_IMAGE_SIZE || bmp.height > MAX_IMAGE_SIZE) {
                        bmp = DrBitmap(0, 0);
                    }
                    
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
                
                // Image dimensions too large! Max width and height are MAX_IMAGE_SIZE!"
                if (bmp.width > MAX_IMAGE_SIZE || bmp.height > MAX_IMAGE_SIZE) {
                    bmp = DrBitmap(0, 0);
                }

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
//##    Atlas Creation
//####################################################################################
// Inits Atlas on GPU, adds a into the Image Manager
std::shared_ptr<DrAtlas>& DrImageManager::addAtlas(Atlas_Type atlas_type, int atlas_size) {
    // Create empty atlas
    std::shared_ptr<DrAtlas> atlas = std::make_shared<DrAtlas>();
        atlas->type = atlas_type;
        atlas->key = atlasKeys().getNextKey();
        atlas->gpu = sg_alloc_image().id;                                           // Alloc an image on the gpu  
        atlas->width = atlas_size;
        atlas->height = atlas_size;

    // Update image on gpu with new empty bitmap data
    DrBitmap bitmap(atlas->width, atlas->height, DROP_BITMAP_FORMAT_ARGB);
    sg_image_desc image_desc { };
        initializeSgImageDesc(bitmap.width, bitmap.height, image_desc);
        image_desc.data.subimage[0][0].ptr = &bitmap.data[0];
        image_desc.data.subimage[0][0].size = static_cast<size_t>(bitmap.size());
    sg_init_image({static_cast<uint32_t>(atlas->gpu)}, &image_desc);

    // Add to atlases
    if (atlas_type == ATLAS_TYPE_SINGLE) {
        m_atlas_single[atlas->key] = atlas;
        return m_atlas_single[atlas->key];
    } else {
        m_atlas_multi[atlas->key] = atlas;
        return m_atlas_multi[atlas->key];
    }    
}


//####################################################################################
//##    Atlas Finding
//####################################################################################
// Packs new Image onto an available Atlas
void DrImageManager::findAtlasForImage(ImageLoadData& image_data) {
    int minimum_size = RoundPowerOf2(image_data.image->bitmap().maxDimension());

    // Single Image
    if (image_data.atlas_type == ATLAS_TYPE_SINGLE) {
        std::shared_ptr<DrAtlas> atlas = addAtlas(image_data.atlas_type, minimum_size);
        addImageToAtlas(image_data, atlas);

    // Multi Image
    } else {
        // Loop through available atlases, attempt to pack
        for (auto& pair : m_atlas_multi) {
            std::shared_ptr<DrAtlas>& atlas = pair.second;
            // Make sure atlas type matches
            if (atlas->type == image_data.atlas_type) {
                // Attempt to add to atlas, if successful we're done
                if (addImageToAtlas(image_data, atlas)) return;                         
            }
        }

        // Didn't find atlas that could fit image, make new one
        std::shared_ptr<DrAtlas> atlas = addAtlas(image_data.atlas_type, minimum_size);
        addImageToAtlas(image_data, atlas);
    }
}


//####################################################################################
//##    Atlas Packing
//####################################################################################
// Runs stb rect pack on collection of images and attempts to pack them onto an atlas
bool DrImageManager::packAtlas(std::shared_ptr<DrAtlas>& atlas, std::vector<stbrp_rect>& rects) {
    // If multi image atlas, pack rects
    if (atlas->type != ATLAS_TYPE_SINGLE) {
        std::vector<stbrp_node> nodes(atlas->maxDimension() * 2);
        stbrp_context rect_pack { }; 
        stbrp_init_target(&rect_pack, atlas->width, atlas->height, &nodes[0], nodes.size());
        stbrp_pack_rects(&rect_pack, &rects[0], rects.size());
        
        // Test that all rects were packed
        bool packed = true;
        for (int i = 0; i < rects.size(); ++i) {
            if (rects[i].was_packed == 0) packed = false;
        }
        return packed;
    } else {
        return true;
    }
}

// Attempts to put an Image onto an Atlas, returns true if successful
bool DrImageManager::addImageToAtlas(ImageLoadData& image_data, std::shared_ptr<DrAtlas>& atlas) {
    // Add image to list of atlas image keys, if checks don't pass below this will be removed
    atlas->packed_image_keys.push_back(image_data.image->key());
    
    // Fill rects with image data
    std::vector<stbrp_rect> rects(atlas->packed_image_keys.size());
    for (int i = 0; i < rects.size(); ++i) {
        setStbRect(rects[i], m_images[atlas->packed_image_keys[i]]);
    }

    // If multi image atlas, test if new image fits
    bool packed = false;
    if (image_data.atlas_type != ATLAS_TYPE_SINGLE) {
        // If enough pixels avaiable, first try full repack
        if (image_data.image->bitmap().numberOfPixels() <= atlas->availablePixels()) {
            packed = packAtlas(atlas, rects);
        }

        // If not packed, try to increase Atlas size
        if (packed == false) {
            // Find size that will fit existing atlas plus new image
            int atlas_x2 =  atlas->maxDimension() * 2;
            int min_dimen = atlas->maxDimension() + image_data.image->bitmap().minDimension();
            int max_dimen = image_data.image->bitmap().maxDimension();
            int size_needed = 0;
            if ((min_dimen <= atlas_x2) && (max_dimen <= atlas_x2)) {
                size_needed = Max(min_dimen, max_dimen);
            } else {
                size_needed = atlas->maxDimension() + image_data.image->bitmap().maxDimension();
            }

            // If needed size is within hardware limits, increase atlas size, repack
            if (size_needed <= sg_query_limits().max_image_size_2d && size_needed <= MAX_ATLAS_SIZE) {
                int resize_atlas = RoundPowerOf2(size_needed);
                atlas->width = resize_atlas;
                atlas->height = resize_atlas;
            } else {
                atlas->packed_image_keys.pop_back();
                return false;
            }            
        }
    }

    // If atlas was resized, or not packed yet, pack atlas now
    if (packed == false) {
        packAtlas(atlas, rects);
    }

    // Copy image pixel data to atlas
    DrBitmap bitmap(atlas->width, atlas->height, DROP_BITMAP_FORMAT_ARGB);
    atlas->pixels_used = 0;
    for (int i = 0; i < atlas->packed_image_keys.size(); ++i) {
        DrRect source_rect = m_images[rects[i].id]->bitmap().rect();
        DrPoint dest_point = DrPoint(rects[i].x, rects[i].y);
        DrBitmap::Blit(m_images[rects[i].id]->bitmap(), source_rect, bitmap, dest_point);

        // Add rect pixels to variable tracking how many pixels have been filled
        atlas->pixels_used += m_images[rects[i].id]->bitmap().numberOfPixels();

        // Update image gpu texture id to match Atlas
        m_images[rects[i].id]->setGpuID(atlas->gpu);

        // Update uv texture coordinates
        float x0 = static_cast<float>(rects[i].x) / static_cast<float>(bitmap.width);
        float y0 = static_cast<float>(rects[i].y) / static_cast<float>(bitmap.height);
        m_images[rects[i].id]->setUv0(x0, y0);
        
        float x1 = static_cast<float>(rects[i].x + rects[i].w) / static_cast<float>(bitmap.width);
        float y1 = static_cast<float>(rects[i].y + rects[i].h) / static_cast<float>(bitmap.height);
        m_images[rects[i].id]->setUv1(x1, y1);
    }

    // Update image on gpu with new bitmap data
    if (sg_query_image_info({static_cast<uint32_t>(atlas->gpu)}).slot.state == SG_RESOURCESTATE_VALID) {
        sg_uninit_image({static_cast<uint32_t>(atlas->gpu)});
    }
    sg_image_desc image_desc { };
        initializeSgImageDesc(bitmap.width, bitmap.height, image_desc);
        image_desc.data.subimage[0][0].ptr = &bitmap.data[0];
        image_desc.data.subimage[0][0].size = static_cast<size_t>(bitmap.size());
    sg_init_image({static_cast<uint32_t>(atlas->gpu)}, &image_desc);

    return true;
}



//####################################################################################
//##    Image Creation
//####################################################################################
// Creates DrImage from DrBitmap from top of image loading stack, calls image callback function if there is one and image creation was successful
void DrImageManager::createImage(DrBitmap& bmp) {
    if (bmp.isValid()) {
        // Image data
        ImageLoadData& image_data = m_load_image_stack[0];
        int new_image_key = imageKeys().getNextKey();

        // Create DrImage
        image_data.image = std::make_shared<DrImage>(image_data.image_file, bmp, image_data.outline);
        image_data.image->setKey(new_image_key);

        // Save copy of pointer to Image Manager
        m_images[new_image_key] = image_data.image;
    
        // Pack Image onto an Atlas, store Atlas gpu id on Image
        findAtlasForImage(image_data);

        // If there is a callback function, call it now
        if (image_data.callback != NULL) {
            image_data.callback(image_data.image);
        }
    }

    // Remove image of list to be fetched
    m_load_image_stack.pop_front();
    m_loading_image = false;
}



