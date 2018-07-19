#pragma once

#include "Curve.h"

// B�X�v���C���Ȑ�
class BsplineCurve : public Curve
{
protected:

    int _nknot; // �m�b�g�x�N�g���T�C�Y
    vector<double> _knot; // �m�b�g�x�N�g��

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

    // �m�b�g�x�N�g���ݒ�
    void SetKnotVector(const double* const knot, int size);

public:

    BsplineCurve(int mord, const ControlPoint* const cp, int cp_size, const double* const knot, 
        const GLdouble* const color, GLdouble width = 1.0, double resol = 20);

    virtual vector<Vector3d> GetPositionVectorsByKnots(int splitSegCnt = 1) const;
    virtual vector<Point3dC> GetPointsByKnots(int splitSegCnt = 1) const;

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double t) const override;
    Vector3d GetFirstDiffVector(double t) const override;
    Vector3d GetSecondDiffVector(double t) const override;

    // �t�ϊ�
    std::unique_ptr<Curve> GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, GLdouble width) const override;

    // �ŋߓ_�擾
    NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d& ref) const override;
};

// �ʊϓ_����t�ϊ����ċȐ����擾����
std::unique_ptr<Curve> GetBsplineCurveFromPoints(const vector<Vector3d>& pnts, int ord, const GLdouble* const color, GLdouble width);