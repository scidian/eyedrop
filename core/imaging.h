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

#include "types/bitmap.h"
#include "types/pointf.h"


// Filters types
enum class Image_Filter_Type {              // ApplySinglePixelFilter 'value' argument
    Brightness,                             //      -255 to 255
    Contrast,                               //      -255 to 255
    Saturation,                             //      -255 to 255 
    Hue,                                    //      -360 to 360
    Grayscale,                              //          N/A
    Negative,                               //          N/A
    Opacity,                                //      -255 to 255
};

// Imaging Enumerations
enum class Flood_Fill_Type {
    Compare_4,
    Compare_8,
};


//####################################################################################
//##    Image editing / object finding
//############################
namespace Dr {

    // ***** Filters
    DrBitmap    ApplySinglePixelFilter(Image_Filter_Type filter, const DrBitmap &from_bitmap, int value);

    // ***** Comparison
    bool        CompareBitmaps(const DrBitmap &bitmap1, const DrBitmap &bitmap2);

    // ***** Object Counting / Fill (a la Ravens Project)
    DrBitmap    BlackAndWhiteFromAlpha(const DrBitmap &bitmap, double alpha_tolerance, bool inverse, 
                                       Bitmap_Format desired_format = Bitmap_Format::ARGB);
    void        FillBorder(DrBitmap &bitmap, DrColor fill_color, DrRect rect);
    bool        FindObjectsInBitmap(const DrBitmap &bitmap, std::vector<DrBitmap> &bitmaps, std::vector<DrRect> &rects, 
                                    double alpha_tolerance, bool convert = true);
    DrBitmap    FloodFill(DrBitmap &bitmap, int at_x, int at_y, DrColor fill_color, double tolerance, Flood_Fill_Type type,
                          int &flood_pixel_count, DrRect &flood_rect);

    // ***** Outlining
    std::vector<DrPointF>       OutlinePointList(const DrBitmap &bitmap);
    std::vector<DrPointF>       TraceImageOutline(const DrBitmap &bitmap);

}

#endif // IMAGE_FILTER_H







