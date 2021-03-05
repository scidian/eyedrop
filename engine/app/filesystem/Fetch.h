//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 03 2021
//
//
#ifndef DR_BLANK_H
#define DR_BLANK_H

#include "3rd_party/sokol/sokol_fetch.h"

// Constants
#define     c_max_file_size     (1024 * 1024);

// Enums
enum File_Load_State {
    FILE_LOADSTATE_UNKNOWN = 0,
    FILE_LOADSTATE_SUCCESS,
    FILE_LOADSTATE_FAILED,
    FILE_LOADSTATE_FILE_TOO_BIG,
};


//####################################################################################
//##    DrFetch
//##        Singleton class to handle Sokol Fetch
//############################
class DrFetch
{
public:
    // Constructor / Destructor
    DrFetch();
    ~DrFetch();

    // #################### VARIABLES ####################
private:
    // Fetch Variables
    int                 m_variable              { 0 };                          // Description
        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    int                 getVariable() { return m_variable; }
    void                setVariable(int variable) { m_variable = variable; }

};


#endif  // DR_BLANK_H
