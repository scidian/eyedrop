/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_ECS_ENTITY_MANAGER_H
#define DR_ECS_ENTITY_MANAGER_H

// Includes
#include <array>
#include <queue>
#include "engine/data/Constants.h"


//####################################################################################
//##    DrEntityManager
//##        Keeps track of Systems within Coordinator (ECS World)
//############################
class DrEntityManager
{
	// #################### VARIABLES ####################
private:
	std::queue<EntityID> 					m_available_entities	{ };			// Current Entities in Entity Manager
	std::array<Archetype, MAX_ENTITIES> 	m_archetypes			{ };			// Archetypes of Entities
	EntityID 								m_living_entity_count 	{ 0 };			// Tracks number of active Entities


	// #################### INTERNAL FUNCTIONS ####################
public:
	// Constructor
	DrEntityManager() {
		// Fill a Queue with available ID Keys
		for (EntityID entity = KEY_START; entity < MAX_ENTITIES; ++entity) {
			m_available_entities.push(entity);
		}
	}

	// Return a valid, unused ID Key
	EntityID createEntity() {
		assert(m_living_entity_count < MAX_ENTITIES && "Too many entities in existence!");
		EntityID id = m_available_entities.front();
		m_available_entities.pop();
		++m_living_entity_count;
		return id;
	}

	// Reclaims Entity ID
	void destroyEntity(EntityID entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range!");
		m_archetypes[entity].reset();
		m_available_entities.push(entity);
		--m_living_entity_count;
	}

	// Stores Archetype of Entity for fast lookup
	void setArchetype(EntityID entity, Archetype archetype) {
		assert((entity >= KEY_START && entity < MAX_ENTITIES) && "Entity out of range!");
		m_archetypes[entity] = archetype;
	}

	// Retrieve Entity Archetype
	Archetype getArchetype(EntityID entity) {
		assert((entity >= KEY_START && entity < MAX_ENTITIES) && "Entity out of range!");
		return m_archetypes[entity];
	}

};


#endif	// DR_ECS_ENTITY_MANAGER_H
