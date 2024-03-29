/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_ENGINE_CONSTANTS_H
#define DR_ENGINE_CONSTANTS_H

// Includes
#include <bitset>
#include <cstdint>

// Global Constants
#define DROP_VERSION             2000                               // Eyedrop version number, important for file loading / saving

#define KEY_NONE                    0                               // Value that represents no item selected
#define KEY_START                   1                               // Starting value for key generators

#define MAX_FILE_SIZE     1024 * 1024                               // Used for filebuffers with sokol_fetch
#define MAX_IMAGE_SIZE           2048                               // Max image size for gpu images, 2048 should support 99.9% of devices from year 2010 on
#define MAX_ATLAS_SIZE           8192                               // Stop stb rect pack from working too hard (2048, 4096, 8192, 16384, 32768, etc, also depends on hardware)
#define ATLAS_PADDING               1                               // Padding of empty pixels added around Images before they are copied onto Atlas

#define MAX_ENTITIES            10000                               // Total of number of entities allowed for now
#define MAX_COMPONENTS             32                               // Current maximum number of compoenents (uint_8), used for sizing Signature

// !!!!! #TEMP
#define INSTANCE_X  100
#define INSTANCE_Y  100
#define INSTANCES   (INSTANCE_X * INSTANCE_Y)
// !!!!! END TEMP


// ECS System Type Defs / Using
using EntityID =            std::uint32_t;                          // Unique Entity ID Key
using ComponentID =         std::uint8_t;                           // Identifier for Components
using Archetype =           std::bitset<MAX_COMPONENTS>;            // A bitset to represent a collection of Components (formerly Signature)
using ArrayIndex = 		    size_t;                                 // For referencing Array subscript values
using HashID =              size_t;                                 // This comes from typeid(OBJECT).hash_code() ... identical to Reflect.h's 'TypeHash'


#endif  // DR_ENGINE_CONSTANTS_H
