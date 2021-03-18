//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 10 2021
//
//
#ifndef DR_ECS_COORDINATOR_H
#define DR_ECS_COORDINATOR_H

// Includes
#include "engine/data/Types.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "SystemManager.h"


//####################################################################################
//##    DrCoordinator
//##        Holds / handles ECS World
//############################
class DrCoordinator
{
    // #################### VARIABLES ####################
private:
	DrComponentManager*     m_component_manager;
	DrEntityManager*        m_entity_manager;
	DrEventManager*         m_event_manager;
	DrSystemManager*        m_system_manager;

public:
    ~DrCoordinator() {
        delete m_component_manager;
        delete m_entity_manager;
        delete m_event_manager;
        delete m_system_manager;
    }

	void Init() {
		m_component_manager = new DrComponentManager();
		m_entity_manager =    new DrEntityManager();
		m_event_manager =     new DrEventManager();
		m_system_manager =    new DrSystemManager();
	}


	// #################### Entity Methods ####################
	DrEntity CreateEntity() {
		return m_entity_manager->CreateEntity();
	}

	void DestroyEntity(DrEntity entity) {
		m_entity_manager->DestroyEntity(entity);
		m_component_manager->EntityDestroyed(entity);
		m_system_manager->EntityDestroyed(entity);
	}

	DrArchetype GetEntityType(DrEntity entity) {
		return m_entity_manager->GetArchetype(entity);
	}


	// #################### Component Methods ####################
	template<typename T>
	void RegisterComponent() {
		m_component_manager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(DrEntity entity, T component) {
		m_component_manager->AddComponent<T>(entity, component);

		auto archetype = m_entity_manager->GetArchetype(entity);
		archetype.set(m_component_manager->GetComponentType<T>(), true);
		m_entity_manager->SetArchetype(entity, archetype);
		m_system_manager->EntityArchetypeChanged(entity, archetype);
	}

	template<typename T>
	void RemoveComponent(DrEntity entity) {
		m_component_manager->RemoveComponent<T>(entity);

		auto archetype = m_entity_manager->GetArchetype(entity);
		archetype.set(m_component_manager->GetComponentType<T>(), false);
		m_entity_manager->SetArchetype(entity, archetype);
		m_system_manager->EntityArchetypeChanged(entity, archetype);
	}

	template<typename T>
	T& GetComponent(DrEntity entity) {
		return m_component_manager->GetComponent<T>(entity);
	}

	template<typename T>
	DrComponentType GetComponentType() {
		return m_component_manager->GetComponentType<T>();
	}


	// #################### System Methods ####################
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		return m_system_manager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemArchetype(DrArchetype archetype) {
		m_system_manager->SetArchetype<T>(archetype);
	}


	// #################### Event Methods ####################
	void AddEventListener(EventId eventId, std::function<void(DrEvent&)> const& listener) {
		m_event_manager->AddListener(eventId, listener);
	}

	void SendEvent(DrEvent& event) {
		m_event_manager->SendEvent(event);
	}

	void SendEvent(EventId eventId) {
		m_event_manager->SendEvent(eventId);
	}

};

#endif  // DR_ECS_COORDINATOR_H
