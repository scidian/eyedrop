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

#include "core/geometry/PointF.h"
#include "core/imaging/Bitmap.h"


// Filters types
enum Image_Filter_Type {                    // ApplySinglePixelFilter 'value' argument
    Brightness,                             //      -255 to 255
    Contrast,                               //      -255 to 255
    Saturation,                             //      -255 to 255 
    Hue,                                    //      -360 to 360
    Grayscale,                              //          N/A
    Negative,                               //          N/A
    Opacity,                                //      -255 to 255
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
    static DrBitmap    ApplySinglePixelFilter(Image_Filter_Type filter, const DrBitmap &from_bitmap, int value);

    // ***** Comparison
    static bool        CompareBitmaps(const DrBitmap &bitmap1, const DrBitmap &bitmap2);

    // ***** Object Counting / Fill (a la Ravens Project)
    static DrBitmap    BlackAndWhiteFromAlpha(const DrBitmap &bitmap, double alpha_tolerance, bool inverse, 
                                              Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    static void        FillBorder(DrBitmap &bitmap, DrColor fill_color, DrRect rect);
    static bool        FindObjectsInBitmap(const DrBitmap &bitmap, std::vector<DrBitmap> &bitmaps, std::vector<DrRect> &rects, 
                                           double alpha_tolerance, bool convert = true);
    static DrBitmap    FloodFill(DrBitmap &bitmap, int at_x, int at_y, DrColor fill_color, double tolerance, Flood_Fill_Type type,
                                 int &flood_pixel_count, DrRect &flood_rect);

    // ***** Outlining
    static std::vector<DrPointF>    TraceImageOutline(const DrBitmap &bitmap);

};

#endif // IMAGE_FILTER_H

