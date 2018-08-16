#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "Curve.h"
#include "BsplineCurve.h"

extern Scene* test_scene;

// 曲面基底クラス
class Surface : public Object
{
public:
    // 最近点導出法
    enum NearestSearch
    {
        Project, // 射影法
        Isoline, // アイソライン法
    };

protected:

    int _ordU, _ordV; // 階数
    int _ncpntU, _ncpntV; // 制御点数
    vector<double> _ctrlpX; // 計算用
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;
    vector<double> _weight;

    mutable GLuint _vbo_nor = 0; // 法線用vbo
    mutable GLuint _ibo_nor = 0; // 法線用ibo
    mutable int _nVertex_cache; // 頂点個数

    // 描画範囲パラメータ
    double _min_draw_param_U, _max_draw_param_U;
    double _min_draw_param_V, _max_draw_param_V;

    mutable int _mesh_displayList = 0; // メッシュ用ディスプレイリスト
    double _mesh_width; // メッシュ線の幅

    // 制御点設定
    void SetControlPoint(const ControlPoint* const cp, int size);

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double u, double v) const = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVectorU(double u, double v) const = 0; // 接線ベクトル
    virtual Vector3d GetFirstDiffVectorV(double u, double v) const = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) const = 0; // 2階微分ベクトル
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) const = 0;
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) const = 0;

    // 法線ベクトル取得
    Vector3d GetNormalVector(double u, double v) const { return (GetFirstDiffVectorU(u, v) * GetFirstDiffVectorV(u, v)); }

    // 描画用曲率ベクトル取得
    Vector3d GetCurvatureVector(double u, double v) const;

    // 指定した端の曲線の制御点を取得する
    vector<ControlPoint> GetEdgeCurveControlPoint(SurfaceEdge edge) const;

    // 最近点取得
    NearestPointInfoS GetNearestPointInfoInternal(const Vector3d& ref, const vector<vector<Point3dS>>& startPnts, const NearestSearch search) const;
    // 最近点を取得する(射影法)
    NearestPointInfoS GetNearestPointFromRefByProjectionMethod(const Vector3d& ref, const Point3dS& start) const;
    NearestPointInfoS GetNearestPointWhenParamOver(const Vector3d& ref, double u, double v) const;
    //// 最近点を取得する(アイソライン法)
    //NearestPointInfoS GetNearestPointFromRefByIsolineMethod(const Vector3d& ref, const Point3dS& start) const;

    // メッシュ表示
    virtual void DrawMeshInternal() const { };
    void DrawMesh() const { DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); }); }

    // Σ[i=1tok]Σ[j=1toL] Q(i,j)N[i,n](u)N[j,m](v) を計算する
    Vector3d CalcVectorWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const;

    // Σ[i=1tok]Σ[j=1toL] w(i,j)N[i,n](u)N[j,m](v) を計算する
    double CalcWeightWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const;

private:

    // オブジェクト描画
    void Draw() const override;

    // 制御点描画
    void DrawCPsInternal() const override;

    // 主曲率を取得
    void GetPrincipalCurvatures(double u, double v, double* const max_kappa, double* const min_kappa) const;
    // 平均曲率取得
    double GetMeanCurvature(double u, double v) const;
    // ガウス曲率取得
    double GetGaussianCurvature(double u, double v) const;

public:

    // 指定した端の曲線を取得する
    virtual std::unique_ptr<Curve> GetEdgeCurve(SurfaceEdge edge) const = 0;

    // 指定したパラメータのアイソ曲線を取得する
    virtual std::unique_ptr<Curve> GetIsoCurve(ParamUV const_param, double param, const GLdouble* const color = Color::red, GLdouble width = 3) const ;

    // 参照点からの最近点を取得
    virtual NearestPointInfoS GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search = Project) const = 0;

    // 最近点を取得する(アイソライン法)
    NearestPointInfoS GetNearestPointFromRefByIsolineMethod(const Vector3d& ref, const Point3dS& start) const;

    // 描画範囲を各方向split_num個に分割するような位置ベクトルを取得する
    void GetPositionVectors(vector<vector<Vector3d>>& pnts, int U_split_num, int V_split_num) const;

    // 通過点から逆変換して曲面を取得する
    virtual std::unique_ptr<Surface> GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, GLdouble width) const = 0;

    // 指定方向に指定パラメータ位置で分割した曲面を取得する
    virtual void GetDevidedSurfaces(const ParamUV direction, std::vector<double>& params, std::vector<std::shared_ptr<Surface>>& devided_surfs, const GLdouble* const color) = 0;
    // 指定パラメータ位置でUV方向に分割した曲面を取得する
    virtual void GetDevidedSurfaces(std::vector<double>& u_params, std::vector<double>& v_params, std::vector<vector<std::shared_ptr<Surface>>>& devided_surfs, const GLdouble* const color) = 0;
    // ノット範囲を等分割した曲面を取得する
    virtual void GetDevidedSurfaces(int splitU, int splitV, std::vector<std::vector<std::shared_ptr<Surface>>>& devided_surfs, const GLdouble* const color) = 0;

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};
