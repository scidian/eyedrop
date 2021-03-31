//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Sat Mar 06 2021
//
//
#ifndef DR_EDITOR_TOOLBAR_H
#define DR_EDITOR_TOOLBAR_H

// Includes
#include <vector>

// Forward Declarations
class DrImage;

     
//####################################################################################
//##    Toolbar
//############################
void    ToolbarUI(bool* widgets, std::vector<std::shared_ptr<DrImage>>& images, int menu_height);


#endif // DR_EDITOR_TOOLBAR_H

