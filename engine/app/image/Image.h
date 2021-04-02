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
#include "engine/app/geometry/Vec2.h"
#include "engine/app/image/Bitmap.h"
#include "engine/data/Constants.h"

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
    // Image Data
    int                         m_key                   { KEY_NONE };               // Key handed out by ImageManager
    std::string                 m_simple_name           { "" };                     // Simple name, i.e. "pretty tree 1"
    DrBitmap                    m_bitmap;                                           // Stored image as Bitmap
        
    // Gpu Info (matched to DrAtlas)
    uint32_t                    m_gpu_id                { INVALID_IMAGE };          // Gpu texture ID if image is also stored on gpu
    DrVec2                      m_uv0                   { 0, 0 };                   // Top left corner of image in gpu texture
    DrVec2                      m_uv1                   { 1, 1 };                   // Bottom right corner of image in gpu texture

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
    int                 key() { return m_key; }
    void                setKey(int key) { m_key = key; }

    // Gpu Ids
    uint32_t            gpuID() { return m_gpu_id; }
    void*               imguiID() { return ((void*)(uintptr_t)m_gpu_id); }          // Returns ImGui compatible texture ID
    void                setGpuID(uint32_t id) { m_gpu_id = id; }

    // Texture Coords
    void                setUv0(float x, float y) { m_uv0 = DrVec2(x, y); }
    void                setUv1(float x, float y) { m_uv1 = DrVec2(x, y); }
    const DrVec2&       uv0() { return m_uv0; }
    const DrVec2&       uv1() { return m_uv1; }

    // Image Helper Functions
    void                outlinePoints(float lod);
    bool                outlineCanceled()   { return m_outline_canceled; }
    bool                outlineProcessed()  { return m_outline_processed; }
    void                setSimpleBox();   

};

#endif // DRIMAGE_H











