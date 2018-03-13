#pragma once

#include "BsplineCurve.h"

class NurbsCurve : public BsplineCurve
{
public:

    NurbsCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width);

    // 各種ベクトル取得
    Vector3d GetPositionVector(const double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;
};