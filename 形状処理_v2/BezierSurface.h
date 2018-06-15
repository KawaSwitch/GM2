#pragma once

#include "Surface.h"
#include "ControlPoint.h"

class BezierSurface : public Surface
{
private:

    // 各種ベクトル描画
    void DrawFirstDiffVectorsInternal() const override;
    void DrawSecondDiffVectorsInternal() const override;
    void DrawNormalVectorsInternal() const override;
    void DrawCurvatureVectorsInternal() const override;

    // メッシュ描画
    void DrawMeshInternal() const override;

    // 事前描画
    void PreDraw() const override;

    // バッファオブジェクト
    void CreateVBO() const override;
    void DrawVBO() const override;

    // 指定した端の曲線を取得する
    Curve* GetEdgeCurve(SurfaceEdge edge) const override;

    // 指定パラメータのベクトルを基底関数から算出する
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double)> BasisFuncU,
        function<double(unsigned, unsigned, double)> BasisFuncV) const;

public:

    BezierSurface(int u_mord, int v_mord, const ControlPoint* const cp, int u_cp_size, int v_cp_size, const GLdouble* const color, GLdouble resol = 20);

    // 各種ベクトル取得
    Vector3d GetPositionVector(double u, double v) const override;
    Vector3d GetFirstDiffVectorU(double u, double v) const override;
    Vector3d GetFirstDiffVectorV(double u, double v) const override;
    Vector3d GetSecondDiffVectorUU(double u, double v) const override;
    Vector3d GetSecondDiffVectorUV(double u, double v) const override;
    Vector3d GetSecondDiffVectorVV(double u, double v) const override;

    // 逆変換
    Surface* GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, GLdouble resol) const override;

    // 最近点取得
    NearestPointInfoS GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search = Project) const override;
};