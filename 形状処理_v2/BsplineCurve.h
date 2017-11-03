#pragma once

#include "Curve.h"

// Bスプライン曲線
class BsplineCurve : public Curve
{
private:

    int _nknot; // ノットベクトルサイズ
    vector<double> _knot; // ノットベクトル
    int _nVertex; // 頂点個数

    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    //void DrawBoxInternal() override;
    //void DrawCurvatureVectorsInternal() override;

    // 事前描画
    void PreDraw() override;
    void CreateVBO() override;
    void DrawVBO() override;

    // ノットベクトル設定
    void SetKnotVector(double* knot, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("ノットベクトル設定失敗", "knot-vector size must be over 0.");

        _knot.reserve(size);
        for (int i = 0; i < size; i++)
            _knot.emplace_back(knot[i]);
    }

public:

    BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width);

    Vector3d GetPositionVector(double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;
};
