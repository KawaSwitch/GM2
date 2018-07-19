#pragma once

#include "Curve.h"

class BezierCurve : public Curve
{
private:

    // �e��x�N�g���`�� 
    void DrawFirstDiffVectorsInternal() const override;
    void DrawSecondDiffVectorsInternal() const override;
    void DrawNormalVectorsInternal() const override;
    void DrawCurvatureVectorsInternal() const override;

    // ���O�`��
    void PreDraw() const override;

    // �o�b�t�@�I�u�W�F�N�g
    void CreateVBO() const override;
    void DrawVBO() const override;

public:

    BezierCurve(int mord, const ControlPoint* const cp, int cp_size, const GLdouble* const color, GLdouble width = 1.0, double resol = 20);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double t) const override;
    Vector3d GetFirstDiffVector(double t) const override;
    Vector3d GetSecondDiffVector(double t) const override;

    // �t�ϊ�
    std::unique_ptr<Curve> GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, GLdouble width) const override;

    // �ŋߓ_�擾
    NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d& ref) const override;
};