//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_EVENT_H
#define DR_ECS_EVENT_H

#include "3rd_party/any.h"
#include "engine/data/Types.h"


//####################################################################################
//##    Events
//####################################################################################
// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count) {
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; }
constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count) {
	return fnv1a_32(s, count); }

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
    const ParamId HEIGHT = 		"Events::Window::Resized::HEIGHT"_hash;
}


//####################################################################################
//##    DrEvent
//##        Event
//############################
class DrEvent
{
	// #################### VARIABLES ####################
private:
	EventId 									mType	{ };
	std::unordered_map<EventId, nonstd::any> 	mData	{ };
	
	
	// #################### INTERNAL FUNCTIONS ####################
public:
	DrEvent() = delete;
	explicit DrEvent(EventId type) : mType(type) { }

	template<typename T>
	void SetParam(EventId id, T value) {
		mData[id] = value;
	}

	template<typename T>
	T GetParam(EventId id) {
		return nonstd::any_cast<T>(mData[id]);
	}

	EventId GetType() const {
		return mType;
	}
};

#endif	// DR_ECS_EVENT_H

