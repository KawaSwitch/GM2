#pragma once

#include "Surface.h"
#include "ControlPoint.h"

class BezierSurface : public Surface
{
private:

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

    // �w�肵���[�̋Ȑ����擾����
    std::unique_ptr<Curve> GetEdgeCurve(SurfaceEdge edge) const override;

    // �w��p�����[�^�̃x�N�g�������֐�����Z�o����
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double)> BasisFuncU,
        function<double(unsigned, unsigned, double)> BasisFuncV) const;

public:

    BezierSurface(int u_mord, int v_mord, const ControlPoint* const cp, int u_cp_size, int v_cp_size, const GLdouble* const color, GLdouble resol = 20);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double u, double v) const override;
    Vector3d GetFirstDiffVectorU(double u, double v) const override;
    Vector3d GetFirstDiffVectorV(double u, double v) const override;
    Vector3d GetSecondDiffVectorUU(double u, double v) const override;
    Vector3d GetSecondDiffVectorUV(double u, double v) const override;
    Vector3d GetSecondDiffVectorVV(double u, double v) const override;

    // �t�ϊ�
    std::unique_ptr<Surface> GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, GLdouble resol) const override;

    // �ŋߓ_�擾
    NearestPointInfoS GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search = Project) const override;
};