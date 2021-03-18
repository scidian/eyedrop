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

	// Getters
	template<typename T>
	std::shared_ptr<DrComponentArray<T>> GetComponentArray() {
		HashID hash = typeid(T).hash_code();
		assert(m_component_ids.find(hash) != m_component_ids.end() && "Component not registered before use!");
		return std::static_pointer_cast<DrComponentArray<T>>(m_component_arrays[hash]);
	}


public:
	// Working on grabbing Component by type for Object Inspector
	std::shared_ptr<IComponentArray> GetComponentArrayByType(ComponentID type) {
		for (auto pair : m_component_ids) {
			if (pair.second == type) return m_component_arrays[pair.first];
		}
		return nullptr;
	}

	


	template<typename T>
	void RegisterComponent() {
		HashID hash = typeid(T).hash_code();
		assert(m_component_ids.find(hash) == m_component_ids.end() && "Registering component type more than once!");

		m_component_ids.insert({hash, m_next_component_id});
		m_component_arrays.insert({hash, std::make_shared<DrComponentArray<T>>()});
		++m_next_component_id;
	}

	template<typename T>
	ComponentID GetComponentID() {
		HashID hash = typeid(T).hash_code();
		assert(m_component_ids.find(hash) != m_component_ids.end() && "Component not registered before use!");
		return m_component_ids[hash];
	}

	template<typename T>
	void AddComponent(EntityID entity, T component) {
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(EntityID entity) {
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(EntityID entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(EntityID entity) {
		for (auto const& pair : m_component_arrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

};


#endif	// DR_ECS_COMPONENT_MANAGER_H
