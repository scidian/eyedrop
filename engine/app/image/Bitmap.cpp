//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "3rd_party/stb/stb_image.h"
#include "3rd_party/stb/stb_image_resize.h"
#include "3rd_party/stb/stb_image_write.h"
#include "../core/Math.h"
#include "../geometry/Point.h"
#include "../geometry/PointF.h"
#include "../geometry/PolygonF.h"
#include "../geometry/Rect.h"
#include "Bitmap.h"
#include "Color.h"


//####################################################################################
//##    Constructors
//####################################################################################
DrBitmap::DrBitmap(Bitmap_Format desired_format) {  
    data.clear(); 
    format = desired_format;
    switch (format) {
        case DROP_BITMAP_FORMAT_GRAYSCALE:   channels = 1;   break;
        case DROP_BITMAP_FORMAT_ARGB:        channels = 4;   break;
    }
}
DrBitmap::~DrBitmap() { 
    data.clear(); 
}

DrBitmap::DrBitmap(const DrBitmap& bitmap, Bitmap_Format desired_format) : DrBitmap(bitmap.width, bitmap.height, desired_format) {
    if (bitmap.format == format && data.size() > 0) {
        channels =  bitmap.channels;
        width =     bitmap.width;
        height =    bitmap.height;
        data.resize(width * height * bitmap.channels);                                              // Resize data vector
        memcpy(&data[0], &bitmap.data[0], data.size());                                             // Copy data
    } else {
        for (int x = 0; x < bitmap.width; ++x) {
            for (int y = 0; y < bitmap.height; ++y) {
                this->setPixel(x, y, bitmap.getPixel(x, y));
            }
        }
    }
}

// Create empty bitmap
DrBitmap::DrBitmap(int width_, int height_, Bitmap_Format desired_format) : DrBitmap(desired_format) {
    width =     width_;
    height =    height_;
    data.resize(width * height * channels);
    std::fill(data.begin(), data.end(), 0);
}

DrBitmap::DrBitmap(std::string filename, Bitmap_Format desired_format) {
    loadFromFile(filename, desired_format);
}

DrBitmap::DrBitmap(const unsigned char* from_data, const int& number_of_bytes, bool compressed, int width_, int height_) {
    loadFromMemory(from_data, number_of_bytes, compressed, width_, height_);
}


//####################################################################################
//##    Blit / Copy
//####################################################################################
// Standard blit, does not stretch. Draws source bitmap from src_rect starting at dst_point in destination
// !! For this current implementation, use only positive source rect widths and heights !!
void DrBitmap::Blit(const DrBitmap& source, DrRect& src_rect, DrBitmap& dest, DrPoint& dst_point) {
    int srcx1 = Max(src_rect.left(),    0);
    int srcx2 = Min(src_rect.right(),   source.rect().right());
    int srcy1 = Max(src_rect.top(),     0);
    int srcy2 = Min(src_rect.bottom(),  source.rect().bottom());
    
    int at_y = dst_point.y;
    for (int y = srcy1; y <= srcy2; ++y) {
        int at_x = dst_point.x;
        for (int x = srcx1; x <= srcx2; ++x) {
            DrColor pixel = source.getPixel(x, y);
            if (at_x >= 0 && at_x <= dest.rect().right()) {
                if (at_y >= 0 && at_y <= dest.rect().bottom()) {
                    dest.setPixel(at_x, at_y, pixel);
                }
            }
            ++at_x;
        }
        ++at_y;
    }
}


//####################################################################################
//##    Manipulation
//####################################################################################
DrBitmap DrBitmap::makeCopy() { return (*this); }

DrBitmap DrBitmap::makeCopy(DrRect& from_rect) {
    // Bounds checking
    int check_left = from_rect.left();
    int check_top  = from_rect.top();
    if (check_left < 0) {
        from_rect.width -= abs(check_left);
        from_rect.x     += abs(check_left);
    }
    if (check_top < 0) {
        from_rect.height -= abs(check_top);
        from_rect.y      += abs(check_top);
    }
    if (from_rect.width <= 0 || from_rect.height <= 0) return DrBitmap(0, 0);
    if (from_rect.right() > this->width - 1)   from_rect.width =  width - from_rect.left();
    if (from_rect.bottom() > this->height - 1) from_rect.height = height - from_rect.top();
    if (from_rect.width <= 0 || from_rect.height <= 0) return DrBitmap(0, 0);

    // Create empty DrBitmap
    DrBitmap copy(from_rect.width, from_rect.height, format);

    // Copy source
    int source_x = from_rect.left();
    for (int x = 0; x < copy.width; ++x) {
        int source_y = from_rect.top();
        for (int y = 0; y < copy.height; ++y) {
            copy.setPixel(x, y, this->getPixel(source_x, source_y));
            ++source_y;
        }
        ++source_x;
    }
    return copy;
}

// Returns a clockwise polygon representing a box around this image
DrPolygonF DrBitmap::polygon() const {
    DrPolygonF box;
    box.addPoint( DrPointF(0,               0) );                           // Top Left
    box.addPoint( DrPointF(this->width - 1, 0) );                           // Top Right
    box.addPoint( DrPointF(this->width - 1, this->height - 1) );            // Bottom Right
    box.addPoint( DrPointF(0,               this->height - 1) );            // Bottom Left
    return box;
}

DrRect DrBitmap::rect() const {
    return DrRect(0, 0, width, height);
}

void DrBitmap::clearPixels() {
    std::fill(data.begin(), data.end(), 0);
}

// !!!!! #WARNING: No out of bounds checks are done here for speed!!
DrColor DrBitmap::getPixel(int x, int y) const {
    size_t index = (y * this->width * channels) + (x * channels);
    switch (format) {
        case DROP_BITMAP_FORMAT_GRAYSCALE:  return DrColor(data[index+0], data[index+0], data[index+0], data[index+0]);
        case DROP_BITMAP_FORMAT_ARGB:       return DrColor(data[index+2], data[index+1], data[index+0], data[index+3]);
    }
}

// !!!!! #WARNING: No out of bounds checks are done here for speed!!
void DrBitmap::setPixel(int x, int y, DrColor color) {
    size_t index = (y * this->width * channels) + (x * channels);
    switch (format) {
        case DROP_BITMAP_FORMAT_GRAYSCALE:   
            data[index]   = (color.redF() * 0.2126) + (color.greenF() * 0.7152) + (color.blueF() * 0.0722);
            break;
        case DROP_BITMAP_FORMAT_ARGB:        
            data[index]   = color.blue();
            data[index+1] = color.green();
            data[index+2] = color.red();
            data[index+3] = color.alpha();       
            break;
    }
}


//####################################################################################
//##    Testing Alpha
//####################################################################################
void DrBitmap::fuzzyAlpha() {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            DrColor color = getPixel(x, y);
            if ((color.red() <  10 && color.green() <  10 && color.blue() <  10) ||
                (color.red() > 245 && color.green() > 245 && color.blue() > 245)) {
                switch (format) {
                    case DROP_BITMAP_FORMAT_GRAYSCALE:  color.setRgbF(0, 0, 0, 0);
                    case DROP_BITMAP_FORMAT_ARGB:       color.setAlpha(0);
                }
                setPixel(x, y, color);
            }
        }
    }
}


//####################################################################################
//##    Loading Images
//####################################################################################
void DrBitmap::loadFromFile(std::string filename, Bitmap_Format desired_format) {
    // Load Image
    unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, desired_format);

    // Error Check
    if (ptr == nullptr || width == 0 || height == 0) {
        width = 0; height = 0; 
        return;
    }

    // Copy Image
    data.resize(width * height * channels);                                                     // Resize data vector
    memcpy(&data[0], ptr, data.size());                                                         // Copy data
    stbi_image_free(ptr);                                                                       // Free the loaded pixels
}

void DrBitmap::loadFromMemory(const unsigned char* from_data, const int& number_of_bytes, bool compressed, int width_, int height_) {
    // Load Raw Data
    if (compressed == false) {
        width = width_;
        height = height_;
        data.resize(number_of_bytes);                                                           // Resize data vector
        memcpy(&data[0], from_data, number_of_bytes);                                           // Copy data

    // Load Image
    } else {
        const stbi_uc* compressed_data = reinterpret_cast<const stbi_uc*>(from_data);
        unsigned char* ptr = stbi_load_from_memory(compressed_data, number_of_bytes, &width, &height, &channels, channels);

        // Error Check
        if (ptr == nullptr || width == 0 || height == 0) {
            width = 0; height = 0; 
            return;
        }

        // Copy Image
        data.resize(width * height * channels);                                                 // Resize data vector
        memcpy(&data[0], ptr, data.size());                                                     // Copy data
        stbi_image_free(ptr);                                                                   // Free the loaded pixels
    }
}

// Aligns pixel format (stb ABGR vs QImage ARGB) for stbi_write
void DrBitmap::saveFormat(std::vector<unsigned char>& formatted) {
    formatted.resize(width * height * channels);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            DrColor pixel = getPixel(x, y);

            size_t index = (y * width * channels) + (x * channels);
            formatted[index] =   pixel.red();
            formatted[index+1] = pixel.green();
            formatted[index+2] = pixel.blue();
            formatted[index+3] = pixel.alpha();
        }
    }
}

int DrBitmap::saveAsBmp(std::string filename) {
    std::vector<unsigned char> formatted;
    saveFormat(formatted);
    int result = stbi_write_bmp(filename.c_str(), width, height, channels, formatted.data());
    return result;
}

int DrBitmap::saveAsJpg(std::string filename, int quality) {
    std::vector<unsigned char> formatted;
    saveFormat(formatted);
    int result = stbi_write_jpg(filename.c_str(), width, height, channels, formatted.data(), quality);
    return result;
}

int DrBitmap::saveAsPng(std::string filename) {
    std::vector<unsigned char> formatted;
    saveFormat(formatted);
    int result = stbi_write_png(filename.c_str(), width, height, channels, formatted.data(), width * channels);
    return result;
}







