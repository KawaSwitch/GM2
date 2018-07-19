#pragma once

#include "BsplineCurve.h"

class NurbsCurve : public BsplineCurve
{
public:

    NurbsCurve(int mord, const ControlPoint* const cp, int cp_size, const double* const knot, const GLdouble* const color, GLdouble width);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double t) const override;
    Vector3d GetFirstDiffVector(double t) const override;
    Vector3d GetSecondDiffVector(double t) const override;
};