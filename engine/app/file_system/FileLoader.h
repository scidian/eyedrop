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
#include <string>
#include "engine/data/Types.h"

// Forward Declarations
class DrBitmap;
class DrImage;
struct sg_image_desc;

//####################################################################################
//##    Image Loading Struct
//############################
struct ImageData {
    std::shared_ptr<DrImage>&   image;
    std::string                 image_file;
    bool                        outline;
};

    
//####################################################################################
//##    DrFileLoader
//##        Loads files for use in App
//############################
class DrFileLoader 
{
public:
    // Constructor / Destructor
    DrFileLoader() { }
    ~DrFileLoader() { }

private:
    // #################### VARIABLES ####################
    uint8_t                         m_load_image_buffer[MAX_FILE_SIZE];             // Buffer to use to load images
    std::deque<ImageData>           m_load_image_stack      { };                    // Stack of images to fetch
    bool                            m_loading_image         { false };              // True when waiting for fetch to complete

public:
    // #################### HELPERS ####################
    static void initializeSgImage(const int& width, const int& height, sg_image_desc& image_desc);

    // #################### IMAGE ####################
    void        addImageToFetch(std::shared_ptr<DrImage>& load_to, std::string image_file, bool outline = false);
    void        createImage(DrBitmap& bmp);
    void        fetchNextImage();


};

#endif  // DR_APP_FILE_LOADER_H
