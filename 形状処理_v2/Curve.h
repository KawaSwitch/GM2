#pragma once

#include "Object.h"
#include "ControlPoint.h"

// 曲線インターフェース
class Curve : public Object
{
protected:

    int _ord; // 階数
    int _ncpnt; // 制御点数
    int _nVertex; // 頂点個数
    double _width; // 曲線の幅
    double _min_draw_param, _max_draw_param; // 描画範囲パラメータ

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double t) = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVector(double t) = 0; // 接線ベクトル
    virtual Vector3d GetSecondDiffVector(double t) = 0; // 2階微分ベクトル

    // 法線ベクトル取得
    Vector3d GetNormalVector(double t) { return (Vector3d(0, 0, 1) * GetFirstDiffVector(t)); }

    // 曲率ベクトル取得
    Vector3d GetCurvatureVector(double t);

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size);

private:

    // 制御点描画
    void DrawCPsInternal() override;

public:

    // 参照点からの最近点を取得する
    Vector3d GetNearestPointFromRef(Vector3d ref);

    // 描画範囲をsplit_num個に分割するような位置ベクトルを取得する
    vector<Vector3d> GetPositionVectors(int split_num);

    // 通過点から逆変換して曲線を取得する
    virtual Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) = 0;

    virtual ~Curve() { };
};