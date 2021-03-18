//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_EVENT_MANAGER_H
#define DR_ECS_EVENT_MANAGER_H

#include <list>
#include "engine/data/Types.h"
#include "Event.h"


//####################################################################################
//##    DrEventManager
//##        Keeps track of Events within Coordinator (ECS World)
//############################
class DrEventManager
{
	// #################### VARIABLES ####################
private:
	std::unordered_map<EventId, std::list<std::function<void(DrEvent&)>>>	listeners;	


	// #################### INTERNAL FUNCTIONS ####################
public:
	void addListener(EventId eventId, std::function<void(DrEvent&)> const& listener) {
		listeners[eventId].push_back(listener);
	}

	void sendEvent(DrEvent& event) {
		uint32_t type = event.getType();

		for (auto const& listener : listeners[type]) {
			listener(event);
		}
	}

	void sendEvent(EventId eventId) {
		DrEvent event(eventId);

		for (auto const& listener : listeners[eventId]) {
			listener(event);
		}
	}

};

#endif	// DR_ECS_EVENT_MANAGER_H


