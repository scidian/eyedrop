//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 22 2021
//
//
#ifndef DR_ASSET_H
#define DR_ASSET_H


//####################################################################################
//##    DrAsset
//##        Base class of every non-ECS object within data structure
//############################
class DrAsset
{
public:
    // Constructor / Destructor
    DrAsset();
    ~DrAsset();

    // #################### VARIABLES ####################
private:
    // Local Variables
    int                 m_variable              { 0 };                              // Description
        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    int                 getVariable() { return m_variable; }
    void                setVariable(int variable) { m_variable = variable; }

};

#endif  // DR_ASSET_H

