//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#ifndef DR_APP_LOAD_IMAGE_H
#define DR_APP_LOAD_IMAGE_H

// Include
#include <string>

// Forward Declarations
class DrImage;


//####################################################################################
//##    Image Fetching
//############################
void        AddImageToFetch(std::shared_ptr<DrImage>& load_to, std::string image_file, bool outline = false);
void        FetchNextImage();


#endif  // DR_APP_LOAD_IMAGE_H
