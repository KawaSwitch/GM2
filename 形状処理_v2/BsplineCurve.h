#pragma once

#include "Curve.h"

// Bスプライン曲線
class BsplineCurve : public Curve
{
protected:

    int _nknot; // ノットベクトルサイズ
    vector<double> _knot; // ノットベクトル

    // 各種ベクトル描画
    void DrawFirstDiffVectorsInternal() const override;
    void DrawSecondDiffVectorsInternal() const override;
    void DrawNormalVectorsInternal() const override;
    void DrawCurvatureVectorsInternal() const override;

    // 事前描画
    void PreDraw() const override;

    // バッファオブジェクト
    void CreateVBO() const override;
    void DrawVBO() const override;

    // ノットベクトル設定
    void SetKnotVector(const double* const knot, int size);

public:

    BsplineCurve(int mord, const ControlPoint* const cp, int cp_size, const double* const knot, 
        const GLdouble* const color, GLdouble width = 1.0, double resol = 20);

    virtual vector<Vector3d> GetPositionVectorsByKnots(int splitSegCnt = 1) const;
    virtual vector<Point3dC> GetPointsByKnots(int splitSegCnt = 1) const;

    // 各種ベクトル取得
    Vector3d GetPositionVector(double t) const override;
    Vector3d GetFirstDiffVector(double t) const override;
    Vector3d GetSecondDiffVector(double t) const override;

    // 逆変換
    Curve* GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, GLdouble width) const override;

    // 最近点取得
    NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d& ref) const override;
};
