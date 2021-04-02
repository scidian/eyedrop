//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 31 2021
//
//
#ifndef DR_ENGINE_CONSTANTS_H
#define DR_ENGINE_CONSTANTS_H

// Includes
#include <bitset>
#include <cstdint>


// Global Constants
#define DROP_VERSION             2000                               // Drop Creator version number, important for file loading / saving

#define KEY_NONE                   -1                               // Value that represents no item selected
#define KEY_START                   1                               // Starting value for key generators

#define MAX_FILE_SIZE       512 * 512                               // Used for filebuffers with sokol_fetch
#define MAX_TEXTURE_SIZE         512                               // Max texture size for gpu images (atlases, etc), 2048 should support 99.9% of devices from year 2010 on
#define INVALID_IMAGE               0                               // Used to identify images that have been initialized, but not loaded yet

#define MAX_ENTITIES            10000                               // Total of number of entities allowed for now
#define MAX_COMPONENTS             32                               // Current maximum number of compoenents (uint_8), used for sizing Signature


// ECS System Type Defs / Using
using EntityID =            std::uint32_t;                          // Unique Entity ID Key
using ComponentID =         std::uint8_t;                           // Identifier for Components
using Archetype =           std::bitset<MAX_COMPONENTS>;            // A bitset to represent a collection of Components (formerly Signature)
using ArrayIndex = 		    size_t;                                 // For referencing Array subscript values
using HashID =              size_t;                                 // This comes from typeid(OBJECT).hash_code() ... identical to Reflect.h's 'TypeHash'


#endif  // DR_ENGINE_CONSTANTS_H
