#pragma once

#include "Surface.h"

class BsplineSurface : public Surface
{
protected:

    int _nknotU, _nknotV; // ノットベクトルサイズ
    vector<double> _knotU, _knotV; // ノットベクトル

    // 各種ベクトル描画
    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;

    // メッシュ描画
    void DrawMeshInternal() override;

    // 事前描画
    void PreDraw() override;

    // バッファオブジェクト
    void CreateVBO() override;
    void DrawVBO() override;
    void CreateIBO() override;
    void DrawIBO() override;

    // ノットベクトル設定
    void SetKnotVector(double* knot, int size, vector<double>& _knot);

private:

    // 指定した端の曲線を取得する
    Curve* GetEdgeCurve(SurfaceEdge edge) override;

    // 指定パラメータのベクトルを基底関数から算出する
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double, double *)> BasisFuncU,
        function<double(unsigned, unsigned, double, double *)> BasisFuncV);

public:

    BsplineSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, double* u_knot, double* v_knot, GLdouble* color, GLdouble width);

    // 各種ベクトル取得
    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};
