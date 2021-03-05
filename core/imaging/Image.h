//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DRIMAGE_H
#define DRIMAGE_H

#include <vector>

#include "core/geometry/PointF.h"
#include "core/imaging/Bitmap.h"

// Local Constants
#define         vtr                     std::vector
const double    c_alpha_tolerance =     0.875;


//####################################################################################
//##    DrImage
//##        Class to hold an Image
//############################
class DrImage
{
private:
    // Local Variables
    std::string                 m_simple_name;                                              // Simple name, i.e. "pretty tree 1"
    DrBitmap                    m_bitmap;                                                   // Stored image as DrBitmap

public:
    vtr<vtr<DrPointF>>          m_poly_list;                                                // Stores list of image outline points
    vtr<vtr<vtr<DrPointF>>>     m_hole_list;                                                // Stores list of hole  outline points
    bool                        m_outline_canceled      { false };                          // True when Image Outline has been canceled, when true extrudes in 3D as simple square
    bool                        m_outline_processed     { false };                          // Turns true when autoOutlinePoints() has completed successfully

private:
    // Internal Variables
    std::string                 m_folder_name           { "" };                             // Used for External Images to belong to a category


public:
    // Constructors
    DrImage(std::string image_name, DrBitmap &bitmap, float lod = 0.25, bool outline = true);

    // Settings
    std::string         getName()   { return m_simple_name; }

    // Image Helper Functions
    void                outlinePoints(float lod);
    bool                outlineCanceled()                   { return m_outline_canceled; }
    bool                outlineProcessed()                  { return m_outline_processed; }
    void                setSimpleBox();

    // Getters / Setters
    std::string         getSimplifiedName()                 { return m_simple_name; }
    const DrBitmap&     getBitmap() const                   { return m_bitmap; }


    // Internal Variable Functions
    std::string         getFolderName()                     { return m_folder_name; }
    void                setFolderName(std::string folder)   { m_folder_name = folder; }

};

#endif // DRIMAGE_H











