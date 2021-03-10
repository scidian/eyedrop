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

#include "ComponentArray.h"
#include "Types.h"


//####################################################################################
//##    DrComponentManager
//##        Keeps track of Components within Coordinator (ECS World)
//############################
class DrComponentManager
{
	// #################### VARIABLES ####################
private:
	std::unordered_map<const char*, DrComponentType> 					m_component_types		{ };
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> 	m_component_arrays		{ };
	DrComponentType 													m_next_component_type	{ };


	template<typename T>
	std::shared_ptr<DrComponentArray<T>> GetComponentArray() {
		const char* type_name = typeid(T).name();
		assert(m_component_types.find(type_name) != m_component_types.end() && "Component not registered before use!");
		return std::static_pointer_cast<DrComponentArray<T>>(m_component_arrays[type_name]);
	}


public:
	template<typename T>
	void RegisterComponent() {
		const char* type_name = typeid(T).name();
		assert(m_component_types.find(type_name) == m_component_types.end() && "Registering component type more than once!");

		m_component_types.insert({type_name, m_next_component_type});
		m_component_arrays.insert({type_name, std::make_shared<DrComponentArray<T>>()});
		++m_next_component_type;
	}

	template<typename T>
	DrComponentType GetComponentType() {
		const char* type_name = typeid(T).name();
		assert(m_component_types.find(type_name) != m_component_types.end() && "Component not registered before use!");
		return m_component_types[type_name];
	}

	template<typename T>
	void AddComponent(DrEntity entity, T component) {
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(DrEntity entity) {
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(DrEntity entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(DrEntity entity) {
		for (auto const& pair : m_component_arrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

};


#endif	// DR_ECS_COMPONENT_MANAGER_H
