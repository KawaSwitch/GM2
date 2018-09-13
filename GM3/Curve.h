#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "NearestPointInfo.h"
#include <utility>

// 曲線基底クラス
class Curve : public Object
{
  protected:
    int _ord;                                // 階数
    int _ncpnt;                              // 制御点数
    double _width;                           // 曲線の幅
    double _min_draw_param, _max_draw_param; // 描画範囲パラメータ

    double _length;                                   // 曲線長
    double _draw_vec_length;                          // 描画ベクトル長
    const double _draw_vector_ratio = (double)1 / 20; // 描画ベクトル長の曲線長との比

    // 法線ベクトル取得
    Vector3d GetNormalVector(double t) const { return (Vector3d(0, 0, 1) * GetFirstDiffVector(t)); }
    // 曲率ベクトル取得
    Vector3d GetCurvatureVector(double t) const;

    // 制御点設定
    void SetControlPoint(const ControlPoint *const cp, int size);

    // 最近点取得(2分探索法)
    NearestPointInfoC GetNearestPointInfoInternal(const Vector3d &ref, const vector<Point3dC> &startPnts) const;

  private:
    // 制御点描画
    void DrawCPsInternal() const override;

    // バッファオブジェクト
    virtual void CreateBufferObject() const override;
    virtual void DrawUsingBufferObject() const override;

  public:
    double GetDrawParamRange() const { return _max_draw_param - _min_draw_param; }
    double GetLength(int split = 100) const;

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double t) const = 0;   // 位置ベクトル
    virtual Vector3d GetFirstDiffVector(double t) const = 0;  // 接線ベクトル
    virtual Vector3d GetSecondDiffVector(double t) const = 0; // 2階微分ベクトル

    // 参照点からの最近点を取得する
    virtual NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d &ref) const = 0;
    // 区間内最近点取得(2分探索)
    NearestPointInfoC GetSectionNearestPointInfoByBinary(const Vector3d &ref, double ini_left, double ini_right) const;
    NearestPointInfoC GetSectionNearestPointInfoByBinary(const Vector3d &ref, double ini_left, double ini_right, int split) const;

    // 描画範囲をsplit_num個に分割するような位置ベクトルを取得する
    void GetPositionVectors(vector<Vector3d> &pnts, int split_num) const;

    // 通過点から逆変換して曲線を取得する
    virtual std::shared_ptr<Curve> GetCurveFromPoints(const vector<Vector3d> &pnts, const GLdouble *const color, GLdouble width) const = 0;

    // 他曲線との相違度を計算します
    double CalcFarthestDistant(const Curve *const other) const;
    double CalcDifferency(const Curve *const other) const;
    double CalcDifferency2(const Curve *const other) const;
    bool IsDifferentAtLeast(const Curve *const other, double eps) const;

    // 曲線を反転します
    void Reverse() override;

    // 指定したパラメータ位置で分割した曲線を取得します
    virtual void GetDevidedCurves(std::vector<double> &params, std::vector<std::shared_ptr<Curve>> &devided_curves) = 0;
    // ノット範囲を等分割した曲線を取得する
    virtual void GetDevidedCurves(int split, std::vector<std::shared_ptr<Curve>> &devided_curves) = 0;

    virtual ~Curve(){};
};
