/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Software
// @source      https://github.com/stevinz/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_MATRIX_H
#define DR_MATRIX_H

#include <cmath>
#include "3rd_party/handmade_math.h"
#include "Vec3.h"


//####################################################################################
//##    DrMatrix
//##        A 4x4 Matrix for use in 3D programming
//############################
class DrMatrix
{
public:
    float m[4][4];

public:
    static hmm_m4 identityMatrix();

    DrMatrix() {
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	DrMatrix operator*(DrMatrix const& rhs) {
		DrMatrix result;

		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				float sum = 0.0f;
				for (int i = 0; i < 4; ++i) {
					sum += m[row][i] * rhs.m[i][col];
				}
				result.m[row][col] = sum;
			}
		}
		return result;
	}

};


#endif  // DR_MATRIX