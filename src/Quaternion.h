#pragma once

#include "GV.h"

// 積の定義
Quaternion& operator*(Quaternion& q1, Quaternion& q2);

// クォータニオン->回転行列の計算
void CalcRotateMatrix(double* r, Quaternion q);