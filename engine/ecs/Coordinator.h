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
	DrComponentManager*     mComponentManager;
	DrEntityManager*        mEntityManager;
	DrEventManager*         mEventManager;
	DrSystemManager*        mSystemManager;

public:
    ~DrCoordinator() {
        delete mComponentManager;
        delete mEntityManager;
        delete mEventManager;
        delete mSystemManager;
    }

	void Init() {
		mComponentManager = new DrComponentManager();
		mEntityManager =    new DrEntityManager();
		mEventManager =     new DrEventManager();
		mSystemManager =    new DrSystemManager();
	}


	// #################### Entity Methods ####################
	DrEntity CreateEntity() {
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(DrEntity entity) {
		mEntityManager->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
	}


	// #################### Component Methods ####################
	template<typename T>
	void RegisterComponent() {
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(DrEntity entity, T component) {
		mComponentManager->AddComponent<T>(entity, component);

		auto archetype = mEntityManager->GetArchetype(entity);
		archetype.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetArchetype(entity, archetype);
		mSystemManager->EntityArchetypeChanged(entity, archetype);
	}

	template<typename T>
	void RemoveComponent(DrEntity entity) {
		mComponentManager->RemoveComponent<T>(entity);

		auto archetype = mEntityManager->GetArchetype(entity);
		archetype.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetArchetype(entity, archetype);
		mSystemManager->EntityArchetypeChanged(entity, archetype);
	}

	template<typename T>
	T& GetComponent(DrEntity entity) {
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	DrComponentType GetComponentType() {
		return mComponentManager->GetComponentType<T>();
	}


	// #################### System Methods ####################
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemArchetype(DrArchetype archetype) {
		mSystemManager->SetArchetype<T>(archetype);
	}


	// #################### Event Methods ####################
	void AddEventListener(EventId eventId, std::function<void(DrEvent&)> const& listener) {
		mEventManager->AddListener(eventId, listener);
	}

	void SendEvent(DrEvent& event) {
		mEventManager->SendEvent(event);
	}

	void SendEvent(EventId eventId) {
		mEventManager->SendEvent(eventId);
	}

};

#endif  // DR_ECS_COORDINATOR_H
