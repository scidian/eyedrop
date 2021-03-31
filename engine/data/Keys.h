//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 31 2021
//
//
#ifndef DR_KEY_GENERATOR
#define DR_KEY_GENERATOR

#include "Constants.h"


//####################################################################################
//##    DrKeys
//##        Key generator class
//############################
class DrKeys
{
public:
    // Constructor / Destructor
    DrKeys();
    ~DrKeys();

    // #################### VARIABLES ####################
private:
    // Local Variables
    int                 m_variable              { 0 };                          // Description
        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    int                 getVariable() { return m_variable; }
    void                setVariable(int variable) { m_variable = variable; }

};

#endif  // DR_KEY_GENERATOR

