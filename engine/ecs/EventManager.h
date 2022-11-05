/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_ECS_EVENT_MANAGER_H
#define DR_ECS_EVENT_MANAGER_H

#include <list>
#include "engine/data/Constants.h"
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


