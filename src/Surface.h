#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "Curve.h"
#include "BsplineCurve.h"

extern Scene* test_scene;

// �Ȗʊ��N���X
class Surface : public Object
{
public:
    // �ŋߓ_���o�@
    enum NearestSearch
    {
        Project, // �ˉe�@
        Isoline, // �A�C�\���C���@
    };

protected:

    int _ordU, _ordV; // �K��
    int _ncpntU, _ncpntV; // ����_��
    vector<double> _ctrlpX; // �v�Z�p
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;
    vector<double> _weight;

    mutable GLuint _vbo_nor = 0; // �@���pvbo
    mutable GLuint _ibo_nor = 0; // �@���pibo
    mutable int _nVertex_cache; // ���_��

    // �`��͈̓p�����[�^
    double _min_draw_param_U, _max_draw_param_U;
    double _min_draw_param_V, _max_draw_param_V;

    mutable int _mesh_displayList = 0; // ���b�V���p�f�B�X�v���C���X�g
    double _mesh_width; // ���b�V�����̕�

    // ����_�ݒ�
    void SetControlPoint(const ControlPoint* const cp, int size);

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double u, double v) const = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVectorU(double u, double v) const = 0; // �ڐ��x�N�g��
    virtual Vector3d GetFirstDiffVectorV(double u, double v) const = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) const = 0; // 2�K�����x�N�g��
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) const = 0;
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) const = 0;

    // �@���x�N�g���擾
    Vector3d GetNormalVector(double u, double v) const { return (GetFirstDiffVectorU(u, v) * GetFirstDiffVectorV(u, v)); }

    // �`��p�ȗ��x�N�g���擾
    Vector3d GetCurvatureVector(double u, double v) const;

    // �w�肵���[�̋Ȑ��̐���_���擾����
    vector<ControlPoint> GetEdgeCurveControlPoint(SurfaceEdge edge) const;

    // �ŋߓ_�擾
    NearestPointInfoS GetNearestPointInfoInternal(const Vector3d& ref, const vector<vector<Point3dS>>& startPnts, const NearestSearch search) const;
    // �ŋߓ_���擾����(�ˉe�@)
    NearestPointInfoS GetNearestPointFromRefByProjectionMethod(const Vector3d& ref, const Point3dS& start) const;
    NearestPointInfoS GetNearestPointWhenParamOver(const Vector3d& ref, double u, double v) const;
    //// �ŋߓ_���擾����(�A�C�\���C���@)
    //NearestPointInfoS GetNearestPointFromRefByIsolineMethod(const Vector3d& ref, const Point3dS& start) const;

    // ���b�V���\��
    virtual void DrawMeshInternal() const { };
    void DrawMesh() const { DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); }); }

    // ��[i=1tok]��[j=1toL] Q(i,j)N[i,n](u)N[j,m](v) ���v�Z����
    Vector3d CalcVectorWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const;

    // ��[i=1tok]��[j=1toL] w(i,j)N[i,n](u)N[j,m](v) ���v�Z����
    double CalcWeightWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const;

private:

    // �I�u�W�F�N�g�`��
    void Draw() const override;

    // ����_�`��
    void DrawCPsInternal() const override;

    // ��ȗ����擾
    void GetPrincipalCurvatures(double u, double v, double* const max_kappa, double* const min_kappa) const;
    // ���ϋȗ��擾
    double GetMeanCurvature(double u, double v) const;
    // �K�E�X�ȗ��擾
    double GetGaussianCurvature(double u, double v) const;

public:

    // �w�肵���[�̋Ȑ����擾����
    virtual std::unique_ptr<Curve> GetEdgeCurve(SurfaceEdge edge) const = 0;

    // �w�肵���p�����[�^�̃A�C�\�Ȑ����擾����
    virtual std::unique_ptr<Curve> GetIsoCurve(ParamUV const_param, double param, const GLdouble* const color = Color::red, GLdouble width = 3) const ;

    // �Q�Ɠ_����̍ŋߓ_���擾
    virtual NearestPointInfoS GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search = Project) const = 0;

    // �ŋߓ_���擾����(�A�C�\���C���@)
    NearestPointInfoS GetNearestPointFromRefByIsolineMethod(const Vector3d& ref, const Point3dS& start) const;

    // �`��͈͂��e����split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����
    void GetPositionVectors(vector<vector<Vector3d>>& pnts, int U_split_num, int V_split_num) const;

    // �ʉߓ_����t�ϊ����ċȖʂ��擾����
    virtual std::unique_ptr<Surface> GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, GLdouble width) const = 0;

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};