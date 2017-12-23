#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "Curve.h"

extern Scene* test_scene;

// 曲面インターフェース
class Surface : public Object
{
protected:

    int _ordU, _ordV; // 階数
    int _nVertex; // 頂点個数

    int _ncpntU, _ncpntV; // 制御点数
    vector<double> _ctrlpX; // 計算用
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;

    GLuint _vbo_nor = 0; // 法線用vbo
    GLuint _ibo_nor = 0; // 法線用ibo
    
    // 描画範囲パラメータ
    double _min_draw_param_U, _max_draw_param_U;
    double _min_draw_param_V, _max_draw_param_V;

    int _mesh_displayList = 0; // メッシュ用ディスプレイリスト
    double _mesh_width; // メッシュ線の幅

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size);

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double u, double v) = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVectorU(double u, double v) = 0; // 接線ベクトル
    virtual Vector3d GetFirstDiffVectorV(double u, double v) = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) { return Vector3d(); }; // 2階微分ベクトル
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) { return Vector3d(); };

    // 法線ベクトル取得
    Vector3d GetNormalVector(double u, double v) { return (GetFirstDiffVectorU(u, v) * GetFirstDiffVectorV(u, v)); }

    // 描画用曲率ベクトル取得
    Vector3d GetCurvatureVector(double u, double v);

    // 指定した端の曲線の制御点を取得する
    vector<ControlPoint> GetEdgeCurveControlPoint(SurfaceEdge edge);

    // メッシュ表示
    virtual void DrawMeshInternal() { };
    void DrawMesh() { DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); }); }

private:

    // オブジェクト描画
    void Draw() override;

    // 制御点描画
    void DrawCPsInternal() override;

    // 主曲率を取得
    void GetPrincipalCurvatures(double u, double v, double* max_kappa, double* min_kappa);
    // 平均曲率取得
    double GetMeanCurvature(double u, double v);
    // ガウス曲率取得
    double GetGaussianCurvature(double u, double v);

public:

    // 指定したパラメータのアイソ曲線を取得する
    //virtual Curve* GetIsoCurve(ParamUV direct, double param) = 0;

    // 指定した端の曲線を取得する
    virtual Curve* GetEdgeCurve(SurfaceEdge edge) = 0;

    // 参照点からの最近点を取得
    Vector3d GetNearestPointFromRef(Vector3d ref);

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};