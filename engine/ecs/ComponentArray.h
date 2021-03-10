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
#include "Types.h"


//####################################################################################
//##    IComponentArray
//##        
//############################
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(DrEntity entity) = 0;
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
	std::array<T, MAX_ENTITIES> 			m_component_array	{ };
	std::unordered_map<DrEntity, size_t> 	m_entity_to_index	{ };
	std::unordered_map<size_t, DrEntity> 	m_index_to_entity	{ };
	size_t 									m_size				{ };


	// #################### INTERNAL FUNCTIONS ####################
public:
	// Adds Component to Entity
	void InsertData(DrEntity entity, T component) {
		assert(m_entity_to_index.find(entity) == m_entity_to_index.end() && "Component added to same entity more than once!");

		// Put new entry at end
		size_t new_index = m_size;
		m_entity_to_index[entity] = new_index;
		m_index_to_entity[new_index] = entity;
		m_component_array[new_index] = component;
		++m_size;
	}

	void RemoveData(DrEntity entity) {
		assert(m_entity_to_index.find(entity) != m_entity_to_index.end() && "Removing non-existent component!");

		// Copy element at end into deleted element's place to maintain density
		size_t index_of_removed_entity = m_entity_to_index[entity];
		size_t index_of_last_element = m_size - 1;
		m_component_array[index_of_removed_entity] = m_component_array[index_of_last_element];

		// Update map to point to moved spot
		DrEntity entity_of_last_element = m_index_to_entity[index_of_last_element];
		m_entity_to_index[entity_of_last_element] = index_of_removed_entity;
		m_index_to_entity[index_of_removed_entity] = entity_of_last_element;

		m_entity_to_index.erase(entity);
		m_index_to_entity.erase(index_of_last_element);
		--m_size;
	}

	T& GetData(DrEntity entity) {
		assert(m_entity_to_index.find(entity) != m_entity_to_index.end() && "Retrieving non-existent component!");
		return m_component_array[m_entity_to_index[entity]];
	}

	void EntityDestroyed(DrEntity entity) override {
		if (m_entity_to_index.find(entity) != m_entity_to_index.end()) {
			RemoveData(entity);
		}
	}

};


#endif	// DR_ECS_COMPONENT_ARRAY

