#pragma once

#include "GV.h"

// �ς̒�`
Quaternion& operator*(Quaternion& q1, Quaternion& q2);

// �N�H�[�^�j�I��->��]�s��̌v�Z
void CalcRotateMatrix(double* r, Quaternion q);