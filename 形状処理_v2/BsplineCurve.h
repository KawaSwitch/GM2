#pragma once

#include "Curve.h"

// Bスプライン曲線
class BsplineCurve : public Curve
{
protected:

    int _nknot; // ノットベクトルサイズ
    vector<double> _knot; // ノットベクトル

    // 各種ベクトル描画
    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;

    // 事前描画
    void PreDraw() override;

    // バッファオブジェクト
    void CreateVBO() override;
    void DrawVBO() override;

    // ノットベクトル設定
    void SetKnotVector(double* knot, int size);

public:

    BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width);

    // 各種ベクトル取得
    Vector3d GetPositionVector(double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;

    // 逆変換
    Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) override;
};
