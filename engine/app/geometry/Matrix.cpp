/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "Matrix.h"


//####################################################################################
//##
//##    Matrix Functions
//##
//############################
hmm_m4 DrMatrix::identityMatrix() {
    hmm_m4 m;
    m.Elements[0][0] = 1.0;
    m.Elements[0][1] = 0.0;
    m.Elements[0][2] = 0.0;
    m.Elements[0][3] = 0.0;

    m.Elements[1][0] = 0.0;
    m.Elements[1][1] = 1.0;
    m.Elements[1][2] = 0.0;
    m.Elements[1][3] = 0.0;

    m.Elements[2][0] = 0.0;
    m.Elements[2][1] = 0.0;
    m.Elements[2][2] = 1.0;
    m.Elements[2][3] = 0.0;

    m.Elements[3][0] = 0.0;
    m.Elements[3][1] = 0.0;
    m.Elements[3][2] = 0.0;
    m.Elements[3][3] = 1.0;
    return m;
}
