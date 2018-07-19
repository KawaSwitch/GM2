#pragma once

#include "BsplineSurface.h"

class NurbsSurface : public BsplineSurface
{
private:

    // �w�肵���[�̋Ȑ����擾����
    std::unique_ptr<Curve> GetEdgeCurve(SurfaceEdge edge) const override;

public:

    NurbsSurface(int u_mord, int v_mord,
        const ControlPoint* const cp, int u_cp_size, int v_cp_size,
        const double* const u_knot, const double* const v_knot,
        const GLdouble* const color, GLdouble resol = 20);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double u, double v) const override;
    Vector3d GetFirstDiffVectorU(double u, double v) const override;
    Vector3d GetFirstDiffVectorV(double u, double v) const override;
    Vector3d GetSecondDiffVectorUU(double u, double v) const override;
    Vector3d GetSecondDiffVectorUV(double u, double v) const override;
    Vector3d GetSecondDiffVectorVV(double u, double v) const override;
};