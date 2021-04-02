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

// Constants
#define     KEY_SINGLE_GENERATOR    1
#define     KEY_INDEX_DEFAULT       0


//####################################################################################
//##    DrKeys
//##        Key generator class
//############################
class DrKeys
{
public:
    // Constructor / Destructor
    DrKeys(int number_of_generators = KEY_SINGLE_GENERATOR, std::vector<int> key_starts = { }) { 
        for (int i = 0; i < number_of_generators; i++) {
            if (i < key_starts.size()) {
                m_key_generators[i] = key_starts[i];
            } else {
                m_key_generators[i] = KEY_START;
            }
        }
    };
    ~DrKeys() { }

    // #################### VARIABLES ####################
private:
    // Key Variables
    std::map<int, int>  m_key_generators;                                           // Map of key generators to hand out unique keys
        

    // #################### INTERNAL FUNCTIONS ####################
public:
    // Key Generator
    int                 checkCurrentKey(int key_index = KEY_INDEX_DEFAULT)          { return m_key_generators[key_index]; }
    int                 getNextKey(int key_index = KEY_INDEX_DEFAULT)               { return m_key_generators[key_index]++; }
    void                setGenerator(int key, int key_index = KEY_INDEX_DEFAULT)    { m_key_generators[key_index] = key; }

};

#endif  // DR_KEY_GENERATOR

