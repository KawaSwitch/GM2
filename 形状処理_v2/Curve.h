#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "NearestPointInfo.h"

// 曲線基底クラス
class Curve : public Object
{
protected:

    int _ord; // 階数
    int _ncpnt; // 制御点数
    double _width; // 曲線の幅
    double _min_draw_param, _max_draw_param; // 描画範囲パラメータ

    double _length; // 曲線長
    double _draw_vec_length; // 描画ベクトル長
    const double _draw_vector_ratio = (double)1 / 20; // 描画ベクトル長の曲線長との比

    // バッファオブジェクト用
    mutable int _nVertex_cache; // 頂点個数

    // 法線ベクトル取得
    Vector3d GetNormalVector(double t) const { return (Vector3d(0, 0, 1) * GetFirstDiffVector(t)); }
    // 曲率ベクトル取得
    Vector3d GetCurvatureVector(double t) const;

    // 制御点設定
    void SetControlPoint(const ControlPoint* const cp, int size);

    // 最近点取得(2分探索法)
    NearestPointInfoC GetNearestPointInfoInternal(const Vector3d& ref, const vector<Point3dC>& startPnts) const;

private:

    // 制御点描画
    void DrawCPsInternal() const override;

    // 区間内最近点取得(2分探索)
    NearestPointInfoC GetSectionNearestPointInfoByBinary(const Vector3d& ref, double ini_left, double ini_right) const;

public:

    double GetMinDrawParam() const { return this->_min_draw_param; }
    double GetMaxDrawParam() const { return this->_max_draw_param; }
    
    double GetLength(int split = 100) const;

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double t) const = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVector(double t) const = 0; // 接線ベクトル
    virtual Vector3d GetSecondDiffVector(double t) const = 0; // 2階微分ベクトル

    // 参照点からの最近点を取得する
    virtual NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d& ref) const = 0;

    // 描画範囲をsplit_num個に分割するような位置ベクトルを取得する
    vector<Vector3d> GetPositionVectors(int split_num) const;

    // 通過点から逆変換して曲線を取得する
    virtual Curve* GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, GLdouble width) const = 0;

    // 他曲線との相違度を計算します
    double CalcDifferency(const Curve* const other) const;

    virtual ~Curve() { };
};