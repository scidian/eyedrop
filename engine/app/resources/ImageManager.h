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
#include <map>
#include <string>
#include <vector>
#include "engine/data/Keys.h"

// Forward Declarations
class DrAtlas;
class DrBitmap;
class DrImage;

// 3rd Party Forward Declarations
struct sg_image_desc;
struct stbrp_context;

// Type Def / Using
using ImageFunction = std::function<void(std::shared_ptr<DrImage>&)>;

// Constants
enum Image_Keys {
    IMAGE_KEY_ATLAS,
    IMAGE_KEY_IMAGE,
    IMAGE_KEY_TOTAL,
};

// Enums
enum Atlas_Type {
    ATLAS_TYPE_NONE,                                                                // Store image on gpu by itself, not in atlas
    ATLAS_TYPE_ENGINE,                                                              // Store image in atlas for use with Engine (away from Project / Game)
    ATLAS_TYPE_PROJECT,                                                             // Store image in atlas for use with Project / Game
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
class DrImageManager : public DrKeys
{
public:
    // Constructor / Destructor
    DrImageManager(std::vector<int> key_starts = { });
    ~DrImageManager() { }

private:
    // #################### VARIABLES ####################
    // Atlas Variables
    std::vector<std::shared_ptr<DrAtlas>>       m_atlas_multi;                      // Atlases (gpu textures) that hold multiple images
    std::vector<std::shared_ptr<DrAtlas>>       m_atlas_single;                     // Atlases (gpu textures) that hold a singular image

    // Image Tracking
    std::map<int, std::shared_ptr<DrImage>>     m_images;                           // Keeps list of loaded images, stored by DrImage key

    // Fetching Variables
    uint8_t                         m_load_image_buffer[MAX_FILE_SIZE];             // Buffer to use to load images
    std::deque<ImageData>           m_load_image_stack      { };                    // Stack of images to fetch
    bool                            m_loading_image         { false };              // True when waiting for fetch to complete
    
public:
    // #################### FUNCTIONS ####################
    // Static Helpers
    static void initializeSgImageDesc(const int& width, const int& height, sg_image_desc& image_desc);

    // Atlas Stuff
    std::shared_ptr<DrAtlas>&   addAtlas(Atlas_Type atlas_type, int atlas_size);
    bool                        addImageToAtlas(ImageData& image_data, std::shared_ptr<DrAtlas>& atlas);
    void                        findAtlasForImage(ImageData& image_data);

    // Image Loading
    void        addImageToFetch(ImageData image_data);
    void        createImage(DrBitmap& bmp);
    void        fetchNextImage();


};

#endif  // DR_APP_IMAGE_MANAGER_H
