/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
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

