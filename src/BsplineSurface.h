#pragma once

#include "Surface.h"

class BsplineSurface : public Surface
{
protected:

    int _nknotU, _nknotV; // �m�b�g�x�N�g���T�C�Y
    vector<double> _knotU, _knotV; // �m�b�g�x�N�g��

    // �e��x�N�g���`��
    void DrawFirstDiffVectorsInternal() const override;
    void DrawSecondDiffVectorsInternal() const override;
    void DrawNormalVectorsInternal() const override;
    void DrawCurvatureVectorsInternal() const override;

    // ���b�V���`��
    void DrawMeshInternal() const override;

    // ���O�`��
    void PreDraw() const override;

    // �o�b�t�@�I�u�W�F�N�g
    void CreateVBO() const override;
    void DrawVBO() const override;
    void CreateIBO() const override;
    void DrawIBO() const override;

    // �m�b�g�x�N�g���ݒ�
    void SetKnotVector(const double* const knot, int size, vector<double>& _knot);

private:

    // �w�肵���[�̋Ȑ����擾����
    std::unique_ptr<Curve> GetEdgeCurve(SurfaceEdge edge) const override;

    // �w��p�����[�^�̃x�N�g�������֐�����Z�o����
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double, const double* const)> BasisFuncU,
        function<double(unsigned, unsigned, double, const double* const)> BasisFuncV) const;

public:

    BsplineSurface(int u_mord, int v_mord,
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

    // �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
    void GetPointsByKnots(vector<vector<Vector3d>>& pnts, int splitSegCnt_U = 1, int splitSegCnt_V = 1) const;
    // �m�b�g�x�N�g�������Ƃɂ��ē_�Q�����擾����
    void GetPointsInfoByKnots(vector<vector<Point3dS>>& pnts, int splitSegCnt_U = 1, int splitSegCnt_V = 1) const;

    // �t�ϊ�
    std::unique_ptr<Surface> GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, GLdouble resol) const override;

    // �ŋߓ_�擾
    NearestPointInfoS GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search = Project) const override;
};

// �ʉߓ_����t�ω�����B�X�v���C���Ȗʂ��擾����
std::unique_ptr<Surface> GetBsplineSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, int ordU, int ordV, const GLdouble* const color, GLdouble resol);