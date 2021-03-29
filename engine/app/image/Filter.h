//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include "../geometry/PointF.h"
#include "Bitmap.h"


// Filters types
enum Image_Filter_Type {              // ApplySinglePixelFilter 'value' argument
    DROP_IMAGE_FILTER_BRIGHTNESS,           //      -255 to 255
    DROP_IMAGE_FILTER_CONTRAST,             //      -255 to 255
    DROP_IMAGE_FILTER_SATURATION,           //      -255 to 255 
    DROP_IMAGE_FILTER_HUE,                  //      -360 to 360
    DROP_IMAGE_FILTER_GRAYSCALE,            //          N/A
    DROP_IMAGE_FILTER_NEGATIVE,             //          N/A
    DROP_IMAGE_FILTER_OPACITY,              //      -255 to 255
};

// Imaging Enumerations
enum Flood_Fill_Type {
    DROP_FLOOD_FILL_COMPARE_4,
    DROP_FLOOD_FILL_COMPARE_8,
};


//####################################################################################
//##    DrFilter
//##        STATIC CLASS: Image editing / object finding
//############################
class DrFilter 
{
public:

    // ***** Filters
    static DrBitmap    applySinglePixelFilter(Image_Filter_Type filter, const DrBitmap& from_bitmap, int value);

    // ***** Comparison
    static bool        compareBitmaps(const DrBitmap& bitmap1, const DrBitmap& bitmap2);

    // ***** Object Counting / Fill (a la Ravens Project)
    static DrBitmap    blackAndWhiteFromAlpha(const DrBitmap& bitmap, double alpha_tolerance, bool inverse, 
                                              Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    static void        fillBorder(DrBitmap& bitmap, DrColor fill_color, DrRect rect);
    static bool        findObjectsInBitmap(const DrBitmap& bitmap, std::vector<DrBitmap>& bitmaps, std::vector<DrRect>& rects, 
                                           double alpha_tolerance, bool convert = true);
    static DrBitmap    floodFill(DrBitmap& bitmap, int at_x, int at_y, DrColor fill_color, double tolerance, Flood_Fill_Type type,
                                 int& flood_pixel_count, DrRect& flood_rect);

    // ***** Outlining
    static std::vector<DrPointF>    traceImageOutline(const DrBitmap &bitmap);

};

#endif // IMAGE_FILTER_H

