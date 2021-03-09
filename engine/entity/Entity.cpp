//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Tue Mar 09 2021
//
//
#include "Entity.h"


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrEntity::DrEntity(std::string name_, long parent_id_) : name(name_), parent_id(parent_id_) {
    
}

DrEntity::~DrEntity() { }

