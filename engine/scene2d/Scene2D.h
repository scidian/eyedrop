//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_SCENE2D_H
#define DR_SCENE2D_H

// Includes
#include <map>

// Forward Declarations
class Object2D;


//####################################################################################
//##    DrScene2D
//##        Holds a 2D scene, with 2D objects
//############################
class DrScene2D 
{
public:
    // Constructor / Destructor
    DrScene2D();
    ~DrScene2D();

    // #################### VARIABLES ####################
private:
    // Scene2D Variables
    std::map<long, DrEntity*>       children            {};
        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    //int                 getVariable() { return m_variable; }
    //void                setVariable(int variable) { m_variable = variable; }

};


#endif  // DR_SCENE2D_H