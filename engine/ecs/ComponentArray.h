//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_COMPONENT_ARRAY_H
#define DR_ECS_COMPONENT_ARRAY_H

// Includes
#include <unordered_map>
#include "engine/data/Constants.h"

//####################################################################################
//##    IComponentArray
//##        
//############################
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(EntityID entity) = 0;
	virtual void* getDataPointer(EntityID entity) = 0;
};


//####################################################################################
//##    ComponentArray
//##        
//############################
template<typename T>
class DrComponentArray : public IComponentArray
{
	// #################### VARIABLES ####################
private:
	std::array<T, MAX_ENTITIES> 				m_component_array	{ };
	std::unordered_map<EntityID, ArrayIndex> 	m_entity_to_index	{ };
	std::unordered_map<ArrayIndex, EntityID> 	m_index_to_entity	{ };
	ArrayIndex   								m_size				{ 0 };


	// #################### INTERNAL FUNCTIONS ####################
public:
	// Called from coordinator when Entity is being removed from Entity Component System
	void entityDestroyed(EntityID entity) override {
		if (m_entity_to_index.find(entity) != m_entity_to_index.end()) {
			removeData(entity);
		}
	}

	// Gets instance of a Component for Entity
	T& getData(EntityID entity) {
		//assert(m_entity_to_index.find(entity) != m_entity_to_index.end() && "Retrieving non-existent component!");
		assert((entity >= KEY_START && entity < MAX_ENTITIES) && "Entity out of range!");
		return m_component_array[m_entity_to_index[entity]];
	}

	// Returns instance of a Component as void* for Entity
	void* getDataPointer(EntityID entity) override {
		return ((void*)(&getData(entity)));
	}

	// Adds a Component to the ComponentArray for an Entity
	void insertData(EntityID entity, T component) {
		assert(m_entity_to_index.find(entity) == m_entity_to_index.end() && "Component added to same entity more than once!");

		// Put new entry at end
		ArrayIndex new_index = m_size;
		m_entity_to_index[entity] = new_index;
		m_index_to_entity[new_index] = entity;
		m_component_array[new_index] = component;
		++m_size;
	}

	// Removes Component for Entity
	void removeData(EntityID entity) {
		assert(m_entity_to_index.find(entity) != m_entity_to_index.end() && "Removing non-existent component!");

		// Copy element at end into deleted element's place to maintain density
		ArrayIndex index_of_removed_entity = m_entity_to_index[entity];
		ArrayIndex index_of_last_element = m_size - 1;
		m_component_array[index_of_removed_entity] = m_component_array[index_of_last_element];

		// Update map to point to moved spot
		EntityID entity_of_last_element = m_index_to_entity[index_of_last_element];
		m_entity_to_index[entity_of_last_element] = index_of_removed_entity;
		m_index_to_entity[index_of_removed_entity] = entity_of_last_element;

		m_entity_to_index.erase(entity);
		m_index_to_entity.erase(index_of_last_element);
		--m_size;
	}

};


#endif	// DR_ECS_COMPONENT_ARRAY

