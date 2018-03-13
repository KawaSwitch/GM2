#pragma once

#include "Curve.h"

class BezierCurve : public Curve
{
private:

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

public:

    BezierCurve(int mord, ControlPoint* cp, int cp_size, GLdouble* color, GLdouble width = 1.0, double resol = 20);

    // 各種ベクトル取得
    Vector3d GetPositionVector(const double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;

    // 逆変換
    Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) override;
};