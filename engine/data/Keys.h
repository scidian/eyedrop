/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
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

