#pragma once

#include "Curve.h"

class BezierCurve : public Curve
{
    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;

    // Ž–‘O•`‰æ
    void PreDraw() override;
    void CreateVBO() override;
    void DrawVBO() override;

public:

    BezierCurve(int mord, ControlPoint* cp, int cp_size, GLdouble* color, GLdouble width);

    Vector3d GetPositionVector(double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;

    Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) override; // ‹t•ÏŠ·
};