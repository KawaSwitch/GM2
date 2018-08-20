#pragma once

#include "Curve.h"

class BezierCurve : public Curve
{
  private:
    // 各種ベクトル描画
    void DrawFirstDiffVectorsInternal() const override;
    void DrawSecondDiffVectorsInternal() const override;
    void DrawNormalVectorsInternal() const override;
    void DrawCurvatureVectorsInternal() const override;

    // 事前描画
    void PreDraw() const override;

    // バッファオブジェクト
    void CreateBufferObject() const override;
    void DrawUsingBufferObject() const override;

  public:
    BezierCurve(int mord, const ControlPoint *const cp, int cp_size, const GLdouble *const color, GLdouble width = 1.0, double resol = 20);

    // 各種ベクトル取得
    Vector3d GetPositionVector(double t) const override;
    Vector3d GetFirstDiffVector(double t) const override;
    Vector3d GetSecondDiffVector(double t) const override;

    // 逆変換
    std::unique_ptr<Curve> GetCurveFromPoints(const vector<Vector3d> &pnts, const GLdouble *const color, GLdouble width) const override;

    // 最近点取得
    NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d &ref) const override;

    // 指定したパラメータ位置で分割した曲線を取得します
    void GetDevidedCurves(std::vector<double> &params, std::vector<std::shared_ptr<Curve>> &devided_curves) override;
    // ノット範囲を等分割した曲線を取得する
    void GetDevidedCurves(int split, std::vector<std::shared_ptr<Curve>> &devided_curves) override;
};
