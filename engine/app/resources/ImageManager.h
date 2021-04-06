//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#ifndef DR_APP_IMAGE_MANAGER_H
#define DR_APP_IMAGE_MANAGER_H

// Include
#include <deque>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "engine/data/Keys.h"

// Forward Declarations
class DrAtlas;
class DrBitmap;
class DrImage;

// 3rd Party Forward Declarations
struct sg_image_desc;
struct stbrp_rect;

// Type Def / Using
using ImageFunction = std::function<void(std::shared_ptr<DrImage>&)>;

// Enums
enum Atlas_Type {
    ATLAS_TYPE_SINGLE,                                                              // Store image on gpu by itself, not in atlas with other images
    ATLAS_TYPE_ENGINE,                                                              // Store image in atlas for use with Engine (internal usage away from Project)
    ATLAS_TYPE_2D_GAME,                                                             // Store image in atlas for use with Project / Game, in 2D Worlds
    ATLAS_TYPE_3D_GAME,                                                             // Store image in atlas for use with Project / Game, in 3D Worlds
    ATLAS_TYPE_UI,                                                                  // Store image in atlas for use with Project / Game, for GUI usage
    ATLAS_TYPE_BACKGROUND,                                                          // Store image in atlas for use with Project / Game, for background tiling
};

//####################################################################################
//##    Image Loading Struct
//############################
struct ImageData {
    ImageData(std::shared_ptr<DrImage>& load_to, std::string file, Atlas_Type atlas, 
              ImageFunction callback_func = NULL, bool perform_outline = false, bool drag_drop = false) :
        image(load_to),
        image_file(file),
        atlas_type(atlas),
        callback(callback_func),
        outline(perform_outline),
        was_dropped(drag_drop) 
    { }
    std::shared_ptr<DrImage>&       image;                                          // DrImage pointer to load new DrImage into after loading
    std::string                     image_file;                                     // File name and path on disk
    Atlas_Type                      atlas_type;                                     // How to store image
    ImageFunction                   callback;                                       // Function to call after loading
    bool                            outline;                                        // Should we run outline function on Image?
    bool                            was_dropped;                                    // Was this file dropped onto window?
};


//####################################################################################
//##    DrImageManager
//##        Loads images for use in App, handles atlases of loaded images
//############################
class DrImageManager
{
public:
    // Constructor / Destructor
    DrImageManager(int atlas_key_start = KEY_START, int image_key_start = KEY_START);
    ~DrImageManager() { }

private:
    // #################### VARIABLES ####################
    // Key Generators
    DrKeys          m_atlas_keys             { };                                   // Key generator for Game Assets
    DrKeys          m_image_keys             { };                                   // Key generator for Game Assets

    // Atlas Variables
    std::unordered_map<int, std::shared_ptr<DrAtlas>>   m_atlas_multi;              // Atlases (gpu textures) that hold multiple images
    std::unordered_map<int, std::shared_ptr<DrAtlas>>   m_atlas_single;             // Atlases (gpu textures) that hold a singular image

    // Image Tracking
    std::unordered_map<int, std::shared_ptr<DrImage>>   m_images;                   // Keeps list of loaded images, stored by DrImage key

    // Fetching Variables
    uint8_t                         m_load_image_buffer[MAX_FILE_SIZE];             // Buffer to use to load images
    std::deque<ImageData>           m_load_image_stack      { };                    // Stack of images to fetch
    bool                            m_loading_image         { false };              // True when waiting for fetch to complete
    
public:
    // #################### FUNCTIONS ####################
    // Static Helpers
    static void initializeSgImageDesc(const int& width, const int& height, sg_image_desc& image_desc);
    static void setStbRect(stbrp_rect& rect, std::shared_ptr<DrImage>& image);

    // Atlas Stuff
    std::shared_ptr<DrAtlas>&   addAtlas(Atlas_Type atlas_type, int atlas_size);
    bool                        addImageToAtlas(ImageData& image_data, std::shared_ptr<DrAtlas>& atlas);
    void                        findAtlasForImage(ImageData& image_data);

    // Image Loading
    void        addImageToFetch(ImageData image_data);
    void        createImage(DrBitmap& bmp);
    void        fetchNextImage();

    // Key Gen
    DrKeys&     atlasKeys()     { return m_atlas_keys; }
    DrKeys&     imageKeys()     { return m_image_keys; }

    // Getters
    std::shared_ptr<DrBitmap>   atlasBitmapFromGpuID(int gpu_id);

};

#endif  // DR_APP_IMAGE_MANAGER_H
