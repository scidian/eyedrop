//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#ifndef DR_APP_FILE_LOADER_H
#define DR_APP_FILE_LOADER_H

// Include
#include <deque>
#include <functional>
#include <string>
#include <vector>
#include "engine/data/Constants.h"

// Forward Declarations
class DrBitmap;
class DrImage;

// 3rd Party Forward Declarations
struct sg_image_desc;
struct stbrp_context;

// Type Def / Using
using ImageFunction = std::function<void(std::shared_ptr<DrImage>&)>;

//####################################################################################
//##    Image Loading Struct
//############################
struct ImageData {
    std::shared_ptr<DrImage>&       image;                                          // DrImage pointer to load new DrImage into after loading
    std::string                     image_file;                                     // File name and path on disk
    ImageFunction                   callback;                                       // Function to call after loading
    bool                            outline;                                        // Should we run outline function on Image?
    bool                            was_dropped;                                    // Was this file dropped onto window?
};
    
//####################################################################################
//##    DrFileLoader
//##        Loads files for use in App
//############################
class DrFileLoader 
{
public:
    // Constructor / Destructor
    DrFileLoader();
    ~DrFileLoader() { }

private:
    // #################### IMAGES ####################
    // Atlas Variables
    std::vector<std::shared_ptr<stbrp_context>> m_rect_packs;                       // Stb Rect Pack Contexts
    std::vector<std::shared_ptr<DrBitmap>>      m_atlases;                          // Holds packed atlases of loaded DrImages
    std::vector<uint32_t>                       m_atlas_ids;                        // GPU Texture IDs for atlases  

    // Image Tracking
    std::vector<std::shared_ptr<DrImage>>       m_loaded_images;                    // Keeps list of loaded images

    // Fetching Variables
    uint8_t                         m_load_image_buffer[MAX_FILE_SIZE];             // Buffer to use to load images
    std::deque<ImageData>           m_load_image_stack      { };                    // Stack of images to fetch
    bool                            m_loading_image         { false };              // True when waiting for fetch to complete
    
public:
    // #################### HELPERS ####################
    static void initializeSgImageDesc(const int& width, const int& height, sg_image_desc& image_desc);

    // #################### IMAGE ####################
    void        addAtlas();
    void        addImageToFetch(std::shared_ptr<DrImage>& load_to, std::string image_file, ImageFunction callback, bool perform_outlin = false, bool was_dropped = false);
    void        createImage(DrBitmap& bmp);
    void        fetchNextImage();


};

#endif  // DR_APP_FILE_LOADER_H
