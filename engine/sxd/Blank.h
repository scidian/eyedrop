//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Mar 01 2021
//
//
#ifndef SXD_BLANK_H
#define SXD_BLANK_H

// Includes
#include <string>

// Namespace
namespace sxd {

// Forward Declarations
class OtherClass;


//####################################################################################
//##    Blank
//##        Blank template header
//############################
class Blank 
{
public:
    // Constructor / Destructor
    Blank();
    ~Blank();

    // #################### VARIABLES ####################
private:
    // Blank Variables
    int                     m_variable          { 0 };                          // Description
        

    // #################### FUNCTIONS TO BE EXPOSED TO API ####################
public:


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions
    int                 getVariable() { return m_variable; }
    void                setVariable(int variable) { m_variable = variable; }

};

}       // end namespace sxd;


#endif  // SXD_BLANK_H