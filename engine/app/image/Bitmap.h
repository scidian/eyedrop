//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef DR_BITMAP_H
#define DR_BITMAP_H

// Includes
#include <string>
#include <vector>

// Forward Declarations
class DrColor;
class DrPolygonF;
class DrRect;

// Local constants
enum Bitmap_Format {
    DROP_BITMAP_FORMAT_GRAYSCALE = 1,
    DROP_BITMAP_FORMAT_ARGB =      4,
};


//####################################################################################
//##    DrBitmap
//##        Holds an image, compatible / loads with stb_image
//############################
class DrBitmap
{
public:
    Bitmap_Format format = DROP_BITMAP_FORMAT_ARGB;     // Bitmap format

    int     channels =  4;                              // Number of 8-bit components per pixel (default is 4: R, G, B, A
    int     width =     0;                              // Image width
    int     height =    0;                              // Image height

    std::vector<unsigned char> data;                    // Pixel data


public:
    // Constructors
    DrBitmap(Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    ~DrBitmap();
    DrBitmap(const DrBitmap& bitmap, Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    DrBitmap(int width_, int height_, Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    DrBitmap(std::string filename, Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    DrBitmap(const unsigned char* from_data, const int& number_of_bytes, 
             bool compressed = true, int width_ = 0, int height_ = 0);

    // Info
    int         size() const { return (width * height * channels); }
    int         maxDimension() const { return ((width > height) ? width : height); }

    // Blit
    static void Blit(const DrBitmap& source, DrRect& src_rect, DrBitmap& dest, DrPoint& dst_point);

    // Manipulation
    DrBitmap    makeCopy();
    DrBitmap    makeCopy(DrRect &from_rect);
    DrPolygonF  polygon() const;
    DrRect      rect() const;
    void        clearPixels();
    DrColor     getPixel(int x, int y) const;
    void        setPixel(int x, int y, DrColor color);

    // Alpha Testing
    void    fuzzyAlpha();

    // Image Loaders
    void    loadFromFile(std::string filename, Bitmap_Format desired_format = DROP_BITMAP_FORMAT_ARGB);
    void    loadFromMemory(const unsigned char* compressed_data, const int& number_of_bytes,
                           bool compressed = true, int width_ = 0, int height_ = 0);


    void    saveFormat(std::vector<unsigned char>& formatted);          // Realigns pixels with stb image format
    int     saveAsBmp(std::string filename);                            // Returns 0 on failure, non-zero on success
    int     saveAsJpg(std::string filename, int quality = 100);         // Returns 0 on failure, non-zero on success, Quality 0-100
    int     saveAsPng(std::string filename);                            // Returns 0 on failure, non-zero on success


    // Functions
    bool    isValid() const     { return (width > 0 && height > 0); }

};


#endif // DR_BITMAP_H









