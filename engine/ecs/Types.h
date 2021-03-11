//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_ENGINE_DATA_TYPES_H
#define DR_ENGINE_DATA_TYPES_H

// STL Includes
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>

// Global Enum Constants
#define DROP_VERSION        2000                            // Drop Creator version number, important for file loading / saving through Cereal

#define KEY_NONE              -1                            // Value that represents no item selected
#define KEY_START              1                            // Starting value for key generators

#define MAX_ENTITIES       10000                            // Total of number of entities allowed for now
#define MAX_COMPONENTS        32                            // Current maximum number of compoenents (uint_8), used for sizing Signature

// ECS System
using DrEntity =            std::uint32_t;                  // Unique Entity ID Key
using DrComponentType =     std::uint8_t;                   // Identifier for Components
using DrArchetype =         std::bitset<MAX_COMPONENTS>;    // A bitset to represent a collection of Components (formerly Signature)



// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count) {
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;     // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count) {
	return fnv1a_32(s, count);
}

// Input
enum class InputButtons
{
	W,
	A,
	S,
	D,
	Q,
	E
};


// Events
using EventId = std::uint32_t;
using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

// TODO: Make these easier to define and use (macro?)
// TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match

namespace Events_Window {
    const EventId QUIT =        "Events::Window::QUIT"_hash;
    const EventId RESIZED =     "Events::Window::RESIZED"_hash;
    const EventId INPUT =       "Events::Window::INPUT"_hash;
}

namespace Events_Window_Input {
    const ParamId INPUT =       "Events::Window::Input::INPUT"_hash;
}

namespace Events_Window_Resized {
    const ParamId WIDTH =       "Events::Window::Resized::WIDTH"_hash;
    const ParamId HEIGHT = "        Events::Window::Resized::HEIGHT"_hash;
}





#endif  // DR_ENGINE_DATA_TYPES_H

