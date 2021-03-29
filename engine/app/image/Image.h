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

#include "engine/app/geometry/PointF.h"
#include "engine/app/image/Bitmap.h"
#include "engine/data/Types.h"

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
    std::string                 m_simple_name           { "" };                     // Simple name, i.e. "pretty tree 1"
    DrBitmap                    m_bitmap;                                           // Stored image as Bitmap
    uint32_t                    m_gpu_id                { 0 };                      // Gpu texture ID if image is also stored on gpu

public:
    vtr<vtr<DrPointF>>          m_poly_list;                                        // Stores list of image outline points (polygons)
    vtr<vtr<vtr<DrPointF>>>     m_hole_list;                                        // Stores list of hole  outline points (list of polygons for each polygon above)
    bool                        m_outline_canceled      { false };                  // True when Image Outline has been canceled, when true extrudes in 3D as simple square
    bool                        m_outline_processed     { false };                  // Turns true when autoOutlinePoints() has completed successfully

private:


public:
    // Constructors
    DrImage(std::string image_name, DrBitmap& bitmap, bool outline = false, float lod = 0.25);

    // Settings
    std::string         name() { return m_simple_name; }
    const DrBitmap&     bitmap() const { return m_bitmap; }
    uint32_t            id() { return m_gpu_id; }
    void*               imguiID() { return ((void*)(uintptr_t)m_gpu_id); }          // Returns ImGui compatible texture ID
    
    // Setters
    void                setID(uint32_t id) { m_gpu_id = id; }

    // Image Helper Functions
    void                outlinePoints(float lod);
    bool                outlineCanceled()   { return m_outline_canceled; }
    bool                outlineProcessed()  { return m_outline_processed; }
    void                setSimpleBox();   

};

#endif // DRIMAGE_H











