/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef GA_ASSET_H
#define GA_ASSET_H


//####################################################################################
//##    GaAsset
//##        Base class of every non-ECS object within App Hierarchy
//############################
class GaAsset
{
public:
    // Constructor / Destructor
    GaAsset();
    ~GaAsset();

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

