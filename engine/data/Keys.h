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

// Includes
#include "Constants.h"


//####################################################################################
//##    DrKeys
//##        Key generator class
//############################
class DrKeys
{
public:
    // Constructor / Destructor
    DrKeys(int key_start = KEY_START) : m_key_generator(key_start) { }
    ~DrKeys() { }

    // #################### VARIABLES ####################
private:
    // Key Variables
    int                 m_key_generator;                                            // Key generator to hand out unique keys
        

    // #################### INTERNAL FUNCTIONS ####################
public:
    // Key Generator
    int                 checkCurrentKey()           { return m_key_generator; }
    int                 getNextKey()                { return m_key_generator++; }
    void                setGenerator(int key)       { m_key_generator = key; }

};

#endif  // DR_KEY_GENERATOR

