//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Tue Mar 09 2021
//
//
#ifndef DR_DECLARES_H
#define DR_DECLARES_H

// Includes
#include <map>
#include <set>
#include <string>
#include <unordered_map>

#include "Types.h"

// Forward Declarations
class DrComponent;                                  // Holds Properties for Entity Components
class DrEntity;                                     // Base class that contains Components
class DrMeta;                                       // Stores meta data about Components / Properties

// Project Types
typedef std::unordered_map<long, std::shared_ptr<DrComponent>>  Components;
typedef std::unordered_map<long, std::shared_ptr<DrEntity>>     Entities;
typedef std::map<Archetype, std::set<long>>                     EntityLists;


#endif  // DR_DECLARES_H