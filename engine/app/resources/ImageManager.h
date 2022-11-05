/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_APP_IMAGE_MANAGER_H
#define DR_APP_IMAGE_MANAGER_H

// Include
#include <deque>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "engine/data/Keys.h"

// Forward Declarations
class DrBitmap;
class DrImage;

// 3rd Party Forward Declarations
struct sg_image_desc;
struct stbrp_rect;

// Type Def / Using
using ImageFunction = std::function<void(std::shared_ptr<DrImage>&)>;

// Enums
enum Atlas_Type {
    ATLAS_TYPE_SINGLE,                                                              // Store image on gpu by itself, not in atlas with other images
    ATLAS_TYPE_ENGINE,                                                              // Store image in atlas for use with Engine (internal usage away from Project)
    ATLAS_TYPE_2D_GAME,                                                             // Store image in atlas for use with Project / Game, in 2D Worlds
    ATLAS_TYPE_3D_GAME,                                                             // Store image in atlas for use with Project / Game, in 3D Worlds
    ATLAS_TYPE_UI,                                                                  // Store image in atlas for use with Project / Game, for GUI usage
    ATLAS_TYPE_BACKGROUND,                                                          // Store image in atlas for use with Project / Game, for background tiling
};


//####################################################################################
//##    Atlas Class
//############################
class DrAtlas {
public:
    // Variables
    Atlas_Type                  type                    { ATLAS_TYPE_SINGLE };      // Type of this Atlas
    int                         width                   { 0 };                      // Width of Atlas
    int                         height                  { 0 };                      // Height of Atlas
    int                         key                     { KEY_NONE };               // Image Manager unique indentifier for this Atlas
    int                         gpu                     { KEY_NONE };               // Texture ID (Atlas in gpu memory)
    std::vector<int>            packed_image_keys       { };                        // Images (image keys) packed onto this Atlas
    int                         pixels_used             { 0 };                      // Total number of pixels used up by Images packed onto this Atlas

    // Functions
    int         availablePixels()       { return ((width * height) - pixels_used); }
    int         maxDimension() const    { return ((width > height) ? width : height); }
};


//####################################################################################
//##    Image Loading Struct
//############################
struct ImageLoadData {
    ImageLoadData(std::shared_ptr<DrImage>& load_to, std::string file, Atlas_Type atlas, int border_padding = 0,
                  ImageFunction callback_func = NULL, bool perform_outline = false, bool drag_drop = false) :
        image(load_to),
        image_file(file),
        atlas_type(atlas),
        padding(border_padding),
        callback(callback_func),
        outline(perform_outline),
        was_dropped(drag_drop)
    { }
    std::shared_ptr<DrImage>&       image;                                          // DrImage pointer to load new DrImage into after loading
    std::string                     image_file;                                     // File name and path on disk
    Atlas_Type                      atlas_type;                                     // How to store image
    int                             padding;                                        // Padding around image when added to Atlas
    ImageFunction                   callback;                                       // Function to call after loading
    bool                            outline;                                        // Should we run outline function on Image?
    bool                            was_dropped;                                    // Was this file dropped onto window?
};


//####################################################################################
//##    DrImageManager
//##        Loads images for use in App, handles atlases of loaded images
//############################
class DrImageManager
{
public:
    // Constructor / Destructor
    DrImageManager(int atlas_key_start = KEY_START, int image_key_start = KEY_START);
    ~DrImageManager() { }

private:
    // #################### VARIABLES ####################
    // Key Generators
    DrKeys          m_atlas_keys             { };                                   // Key generator for Game Assets
    DrKeys          m_image_keys             { };                                   // Key generator for Game Assets

    // Atlas Variables
    std::unordered_map<int, std::shared_ptr<DrAtlas>>   m_atlas_multi;              // Atlases (gpu textures) that hold multiple images
    std::unordered_map<int, std::shared_ptr<DrAtlas>>   m_atlas_single;             // Atlases (gpu textures) that hold a singular image

    // Image Tracking
    std::unordered_map<int, std::shared_ptr<DrImage>>   m_images;                   // Keeps list of loaded images, stored by DrImage key

    // Fetching Variables
    uint8_t                         m_load_image_buffer[MAX_FILE_SIZE];             // Buffer to use to load images
    std::deque<ImageLoadData>       m_load_image_stack      { };                    // Stack of images to fetch
    bool                            m_loading_image         { false };              // True when waiting for fetch to complete

public:
    // #################### FUNCTIONS ####################
    // Static Helpers
    static void initializeSgImageDesc(const int& width, const int& height, sg_image_desc& image_desc);
    static void setStbRect(stbrp_rect& rect, std::shared_ptr<DrImage>& image);

    // Getters
    std::shared_ptr<DrAtlas>&   atlasFromGpuID(int gpu_id);

    // Image Loading
    void        fetchImage(ImageLoadData image_data);
    void        loadImage(ImageLoadData image_data);
    void        processFetchStack();

private:
    // Atlas Creation
    std::shared_ptr<DrAtlas>&   addAtlas(Atlas_Type atlas_type, int atlas_size);
    void                        findAtlasForImage(ImageLoadData& image_data);

    // Atlas Packing
    bool                        addImageToAtlas(ImageLoadData& image_data, std::shared_ptr<DrAtlas>& atlas);
    bool                        packAtlas(std::shared_ptr<DrAtlas>& atlas, std::vector<stbrp_rect>& rects);

    // Image Creation
    void                        createImage(DrBitmap& bmp);

    // Key Gen
    DrKeys&     atlasKeys()     { return m_atlas_keys; }
    DrKeys&     imageKeys()     { return m_image_keys; }

};

#endif  // DR_APP_IMAGE_MANAGER_H
