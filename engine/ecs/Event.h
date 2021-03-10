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
#include "Types.h"


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

