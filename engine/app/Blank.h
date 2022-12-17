/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_BLANK_H
#define DR_BLANK_H


//####################################################################################
//##    Dr
//##        Blank template header
//############################
class DrBlank
{
public:
    // Constructor / Destructor
    DrBlank();
    ~DrBlank();

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

#endif  // DR_BLANK_H


