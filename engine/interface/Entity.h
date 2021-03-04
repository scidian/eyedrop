//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_ENTITY_H
#define DR_ENTITY_H

// Includes
#include <map>

// Forward Declarations
class DrComponent;


//####################################################################################
//##    DrEntity
//##        Interface class to hold a Scene Object
//############################
class DrEntity 
{
public:
    // Constructor / Destructor
    DrEntity();
    ~DrEntity();

    // #################### VARIABLES ####################
private:
    // Local Variables
    std::string                         m_name                      { };                            // Name of this Entity
    std::map<long, DrComponent*>        m_components                { };
        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    //int                 getVariable() { return m_variable; }
    //void                setVariable(int variable) { m_variable = variable; }

};


#endif  // DR_ENTITY_H