//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Wed Mar 03 2021
//
//
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
