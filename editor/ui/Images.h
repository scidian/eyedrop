//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#ifndef DR_EDITOR_IMAGES_H
#define DR_EDITOR_IMAGES_H

// Gui Images
enum Editor_Images {
    EDITOR_IMAGE_NONE               = -1,                                           // Represents no image found / loaded / etc.
    EDITOR_IMAGE_WORLD_GRAPH,
    EDITOR_IMAGE_WORLD_CREATOR,
    EDITOR_IMAGE_UI_CREATOR,
    EDITOR_IMAGE_TOTAL,
};


//####################################################################################
//##    Editor Image Handling
//############################
void        AddImageToLoad(Editor_Images image_number, std::string image_file);
void        FetchNextImage();


#endif // DR_EDITOR_IMAGES_H

