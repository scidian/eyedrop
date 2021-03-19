//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_COMPONENT_MANAGER_H
#define DR_ECS_COMPONENT_MANAGER_H

#include "engine/data/Types.h"
#include "ComponentArray.h"


//####################################################################################
//##    DrComponentManager
//##        Keeps track of Components within Coordinator (ECS World)
//############################
class DrComponentManager
{
	// #################### VARIABLES ####################
private:
	std::unordered_map<HashID, ComponentID> 						m_component_ids			{ };
	std::unordered_map<HashID, std::shared_ptr<IComponentArray>>	m_component_arrays		{ };
	ComponentID 													m_next_component_id		{ 0 };


public:
	// Registers a Component Type with Entity Component System, ex: registerComponent<Transform>();
	template<typename T>
	void registerComponent() {
		HashID hash = typeid(T).hash_code();
		assert(m_component_ids.find(hash) == m_component_ids.end() && "Registering component type more than once!");

		m_component_ids.insert({hash, m_next_component_id});
		m_component_arrays.insert({hash, std::make_shared<DrComponentArray<T>>()});
		++m_next_component_id;
	}

	// Adds a Component of Type T to Entity
	template<typename T>
	void addComponent(EntityID entity, T component) {
		getComponentArray<T>()->insertData(entity, component);
	}

	// Removes a Component of Type T from Entity
	template<typename T>
	void removeComponent(EntityID entity) {
		getComponentArray<T>()->removeData(entity);
	}

	// Returns a Component of Type T from Entity
	template<typename T>
	T& getComponent(EntityID entity) {
		return getComponentArray<T>()->getData(entity);
	}

	// Gets ComponentArray of Type T
	template<typename T>
	std::shared_ptr<DrComponentArray<T>> getComponentArray() {
		HashID hash = typeid(T).hash_code();
		assert(m_component_ids.find(hash) != m_component_ids.end() && "Component not registered before use!");
		return std::static_pointer_cast<DrComponentArray<T>>(m_component_arrays[hash]);
	}

	// For grabbing Component by type (for Object Inspector)
	IComponentArray* getComponentArray(ComponentID component_id) {
		for (auto& pair : m_component_ids) {
			if (pair.second == component_id) {
				return m_component_arrays[pair.first].get();
			}
		}
		assert(false && "Component Array not found!");
	}

	// Gets component id (bitset) of a Component with Type T
	template<typename T>
	ComponentID getComponentID() {
		HashID hash = typeid(T).hash_code();
		assert(m_component_ids.find(hash) != m_component_ids.end() && "Component not registered before use!");
		return m_component_ids[hash];
	}

	// Returns typeid().hash_code() of Component Type
	HashID getComponentHashID(ComponentID component_id) {
		for (auto& pair : m_component_ids) {
			if (pair.second == component_id) {
				HashID hash = pair.first;
				return hash;
			}
		}
		assert(false && "Component ID not found!");
	}

	// Called from Coordinator.destroyEntity()
	void entityDestroyed(EntityID entity) {
		for (auto const& pair : m_component_arrays) {
			auto const& component = pair.second;
			component->entityDestroyed(entity);
		}
	}

};


#endif	// DR_ECS_COMPONENT_MANAGER_H
