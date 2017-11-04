#pragma once

#include "Surface.h"

class BsplineSurface : public Surface
{
private:

    int _nknotU, _nknotV; // ノットベクトルサイズ
    vector<double> _knotU, _knotV; // ノットベクトル

    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;
    void DrawMeshInternal() override;

    // 事前描画
    void PreDraw() override;
    void CreateVBO() override;
    void DrawVBO() override;

    // ノットベクトル設定
    void SetKnotVector(double* knot, int size, vector<double>& _knot)
    {
        if (size <= 0)
            Error::ShowAndExit("ノットベクトル設定失敗", "knot-vector size must be over 0.");

        _knot.reserve(size);
        for (int i = 0; i < size; i++)
            _knot.emplace_back(knot[i]);
    }

public:

    BsplineSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, double* u_knot, double* v_knot, GLdouble* color, GLdouble width);

    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};
