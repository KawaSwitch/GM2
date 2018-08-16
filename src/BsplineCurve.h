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

    // ノット範囲を等分割する位置のノットを取得する
    void GetSplitParam(vector<double>& params, int splitSegCnt = 1);
    
    virtual vector<Vector3d> GetPositionVectorsByKnots(int splitSegCnt = 1) const;
    virtual vector<Point3dC> GetPointsByKnots(int splitSegCnt = 1) const;

    // 各種ベクトル取得
    Vector3d GetPositionVector(double t) const override;
    Vector3d GetFirstDiffVector(double t) const override;
    Vector3d GetSecondDiffVector(double t) const override;

    // 逆変換
    std::unique_ptr<Curve> GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, GLdouble width) const override;

    // 最近点取得
    NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d& ref) const override;

    // ノット追加
    void AddKnot(double t);
    // 指定したパラメータ位置で分割した曲線を取得する
    void GetDevidedCurves(std::vector<double>& params, std::vector<std::shared_ptr<Curve>>& devided_curves) override;
    // ノット範囲を等分割した曲線を取得する
    void GetDevidedCurves(int split, std::vector<std::shared_ptr<Curve>>& devided_curves) override;
};

// 通観点から逆変換して曲線を取得する
std::unique_ptr<Curve> GetBsplineCurveFromPoints(const vector<Vector3d>& pnts, int ord, const GLdouble* const color, GLdouble width);
