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
	// Constructor / Destructor
	DrCoordinator() {
		m_component_manager = new DrComponentManager();
		m_entity_manager =    new DrEntityManager();
		m_event_manager =     new DrEventManager();
		m_system_manager =    new DrSystemManager();
	}
    ~DrCoordinator() {
        delete m_component_manager;
        delete m_entity_manager;
        delete m_event_manager;
        delete m_system_manager;
    }


	// #################### Entity Methods ####################
	EntityID createEntity() {
		return m_entity_manager->createEntity();
	}

	void destroyEntity(EntityID entity) {
		m_entity_manager->destroyEntity(entity);
		m_component_manager->entityDestroyed(entity);
		m_system_manager->entityDestroyed(entity);
	}

	Archetype getEntityType(EntityID entity) {
		return m_entity_manager->getArchetype(entity);
	}


	// #################### Component Methods ####################
	template<typename T>
	void registerComponent() {
		m_component_manager->registerComponent<T>();
	}

	template<typename T>
	void addComponent(EntityID entity, T component) {
		m_component_manager->addComponent<T>(entity, component);

		auto archetype = m_entity_manager->getArchetype(entity);
		archetype.set(m_component_manager->getComponentID<T>(), true);
		m_entity_manager->setArchetype(entity, archetype);
		m_system_manager->entityArchetypeChanged(entity, archetype);
	}

	template<typename T>
	void removeComponent(EntityID entity) {
		m_component_manager->removeComponent<T>(entity);

		auto archetype = m_entity_manager->getArchetype(entity);
		archetype.set(m_component_manager->getComponentID<T>(), false);
		m_entity_manager->setArchetype(entity, archetype);
		m_system_manager->entityArchetypeChanged(entity, archetype);
	}

	template<typename T>
	T& getComponent(EntityID entity) {
		return m_component_manager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentID getComponentID() {
		return m_component_manager->getComponentID<T>();
	}

	HashID getComponentHashID(ComponentID component_id) {
		return m_component_manager->getComponentHashID(component_id);
	}

	// #################### System Methods ####################
	template<typename T>
	std::shared_ptr<T> registerSystem() {
		return m_system_manager->registerSystem<T>();
	}

	template<typename T>
	void setSystemArchetype(Archetype archetype) {
		m_system_manager->setArchetype<T>(archetype);
	}


	// #################### Event Methods ####################
	void addEventListener(EventId eventId, std::function<void(DrEvent&)> const& listener) {
		m_event_manager->addListener(eventId, listener);
	}

	void sendEvent(DrEvent& event) {
		m_event_manager->sendEvent(event);
	}

	void sendEvent(EventId eventId) {
		m_event_manager->sendEvent(eventId);
	}

};

#endif  // DR_ECS_COORDINATOR_H
