//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#include "core/imaging/Bitmap.h"
#include "../Editor.h"
#include "Images.h"


// Image to be Loaded
struct ImageData {
    Editor_Images   image_number = EDITOR_IMAGE_NONE;
    std::string     image_file = "";
};

//####################################################################################
//##    Local Variables
//####################################################################################
uint8_t                     l_editor_image_buffer[MAX_FILE_SIZE];                   // Buffer to use to load images
std::deque<ImageData>       l_image_load_stack      { };                            // Stack of images to fetch
bool                        l_loading_image         { false };                      // True when waiting for fetch to complete


//####################################################################################
//##    Image Fetching
//####################################################################################
// Adds image to stack of images to be loaded
void AddImageToLoad(Editor_Images image_number, std::string image_file) {
    ImageData img_data { };
        img_data.image_number = image_number;
        img_data.image_file = image_file;
    l_image_load_stack.push_back(img_data);
}


// Initiates fetch of next image on the load stack, calls until all images are loaded
void FetchNextImage() {
    if (l_loading_image || l_image_load_stack.size() < 1) return;
    l_loading_image = true;

    sfetch_request_t sokol_fetch_image { };
        sokol_fetch_image.path = l_image_load_stack[0].image_file.c_str();
        sokol_fetch_image.buffer_ptr = l_editor_image_buffer;
        sokol_fetch_image.buffer_size = sizeof(l_editor_image_buffer);
        sokol_fetch_image.callback = +[](const sfetch_response_t* response) {
            // Load Data from response
            DrBitmap bmp((unsigned char*)response->buffer_ptr, (int)response->fetched_size);

            // If valid, create image on gpu
            if (response->error_code == SFETCH_ERROR_FILE_NOT_FOUND /* (1) */) { }
            if (bmp.isValid()) {
                sg_image_desc img_desc { };
                    img_desc.width =        bmp.width;
                    img_desc.height =       bmp.height;
                    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
                    img_desc.wrap_u =       SG_WRAP_CLAMP_TO_EDGE;
                    img_desc.wrap_v =       SG_WRAP_CLAMP_TO_EDGE;
                    img_desc.min_filter =   SG_FILTER_LINEAR;
                    img_desc.mag_filter =   SG_FILTER_LINEAR;
                    img_desc.data.subimage[0][0].ptr = &bmp.data[0];
                    img_desc.data.subimage[0][0].size = static_cast<size_t>(bmp.width * bmp.height * bmp.channels);
                if (l_image_load_stack[0].image_number != EDITOR_IMAGE_NONE) {
                    DrEditor* editor = dynamic_cast<DrEditor*>(g_app);
                    editor->gui_images[l_image_load_stack[0].image_number] = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
                }
            }

            // Remove image of list to be fetched
            l_image_load_stack.pop_front();
            l_loading_image = false;
        };
    sfetch_send(&sokol_fetch_image);    
}

