//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_SYSTEM_MANAGER_H
#define DR_ECS_SYSTEM_MANAGER_H

#include "engine/data/Types.h"
#include "System.h"


//####################################################################################
//##    DrSystemManager
//##        Keeps track of Systems within Coordinator (ECS World)
//############################
class DrSystemManager
{
	// #################### VARIABLES ####################
private:
	std::unordered_map<const char*, std::shared_ptr<DrSystem>> 	m_systems 		{ };	// Current Systems in System Manager
	std::unordered_map<const char*, DrArchetype>     			m_archetypes 	{ };	// Archetypes of Systems
	

	// #################### INTERNAL FUNCTIONS ####################
public:
	// Adds a System to the System Manager
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* type_name = typeid(T).name();
		assert(m_systems.find(type_name) == m_systems.end() && "Registering system more than once!");
		
		// Otherwise, add system to manager
		auto system = std::make_shared<T>();
		m_systems.insert({type_name, system});
		return system;
	}

	// Sets Archetype (a bitset based on desired Components) of a System
	template<typename T>
	void SetArchetype(DrArchetype archetype) {
		const char* type_name = typeid(T).name();
		assert(m_systems.find(type_name) != m_systems.end() && "System used before being registered!");

		m_archetypes.insert({type_name, archetype});
	}

	// Entity has been destroyed, remove from all Systems
	void EntityDestroyed(DrEntity entity) {
		for (auto const& system_pair : m_systems) {
			auto const& system = system_pair.second;
			system->m_entities.erase(entity);
		}
	}
	
	// Check all Systems for Entity
	//		If Entity has necessay components of System (shares Archetype), make sure that Entity is included in that System
	//		Otherwise remove the Entity from that System
	void EntityArchetypeChanged(DrEntity entity, DrArchetype entity_archetype) {
		for (auto const& system_pair : m_systems) {
			auto const& type =   system_pair.first;
			auto const& system = system_pair.second;
			auto const& system_archetype = m_archetypes[type];

			if ((entity_archetype & system_archetype) == system_archetype) {
				system->m_entities.insert(entity);
			} else {
				system->m_entities.erase(entity);
			}
		}
	}

};


#endif	// DR_ECS_SYSTEM_MANAGER_H
