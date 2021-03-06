﻿#pragma once

#include "Surface.h"
#include "Edge.h"

class BsplineSurface : public Surface
{
  protected:
    int _nknotU, _nknotV;          // ノットベクトルサイズ
    vector<double> _knotU, _knotV; // ノットベクトル

    // 各種ベクトル描画
    void DrawFirstDiffVectorsInternal() const override;
    void DrawSecondDiffVectorsInternal() const override;
    void DrawNormalVectorsInternal() const override;
    void DrawCurvatureVectorsInternal() const override;

    // メッシュ描画
    void DrawMeshInternal() const override;

    // 事前描画(ディスプレイリスト用)
    void PreDraw() const override;

    // ノットベクトル設定
    void SetKnotVector(const double *const knot, int size, vector<double> &_knot);

    // 指定方向に指定パラメータ位置で分割した曲面を取得する
    void GetDevidedSurfaces(const ParamUV direction, std::vector<double> &params, std::vector<std::shared_ptr<Surface>> &devided_surfs, const GLdouble *const color) override;

  private:
    // 指定した端の曲線を取得する
    std::unique_ptr<Curve> GetEdgeCurve(SurfaceEdge edge) const override;

    // 指定パラメータのベクトルを基底関数から算出する
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double, const double *const)> BasisFuncU,
        function<double(unsigned, unsigned, double, const double *const)> BasisFuncV) const;

  public:
    BsplineSurface(int u_mord, int v_mord,
                   const ControlPoint *const cp, int u_cp_size, int v_cp_size,
                   const double *const u_knot, const double *const v_knot,
                   const GLdouble *const color, GLdouble resol = 20);

    // 各種ベクトル取得
    Vector3d GetPositionVector(double u, double v) const override;
    Vector3d GetFirstDiffVectorU(double u, double v) const override;
    Vector3d GetFirstDiffVectorV(double u, double v) const override;
    Vector3d GetSecondDiffVectorUU(double u, double v) const override;
    Vector3d GetSecondDiffVectorUV(double u, double v) const override;
    Vector3d GetSecondDiffVectorVV(double u, double v) const override;

    // ノット範囲を等分割する位置のノットを取得する
    void GetSplitParam(ParamUV param, vector<double> &params, int splitSegCnt = 1) const;

    // ノットベクトルをもとにして点群を取得する
    void GetPointsByKnots(vector<vector<Vector3d>> &pnts, int splitSegCnt_U = 1, int splitSegCnt_V = 1) const;
    // ノットベクトルをもとにして点群情報を取得する
    void GetPointsInfoByKnots(vector<vector<Point3dS>> &pnts, int splitSegCnt_U = 1, int splitSegCnt_V = 1) const;

    // 逆変換
    std::unique_ptr<Surface> GetSurfaceFromPoints(const vector<vector<Vector3d>> &pnts, const GLdouble *const color, GLdouble resol) const override;

    // 最近点取得
    NearestPointInfoS GetNearestPointInfoFromRef(const Vector3d &ref, const NearestSearch search = Project) const override;

    // ノットを追加する
    void AddKnot(const ParamUV direction, double param);

    // 指定パラメータ位置でUV方向に分割した曲面を取得する
    void GetDevidedSurfaces(std::vector<double> &u_params, std::vector<double> &v_params, std::vector<vector<std::shared_ptr<Surface>>> &devided_surfs, const GLdouble *const color) override;
    // ノット範囲を等分割した曲面を取得する
    void GetDevidedSurfaces(int splitU, int splitV, std::vector<std::vector<std::shared_ptr<Surface>>> &devided_surfs, const GLdouble *const color) override;

    // 曲面上に乗るような曲線を再生成し位相要素を取得
    std::shared_ptr<Edge> GetOnSurfaceCurve(const std::shared_ptr<BsplineCurve> curve,
        std::vector<Vector3d>& on_params, std::vector<Point<double>>& uv_params) const;

};

// 通過点から逆変換してBスプライン曲面を取得する
std::unique_ptr<Surface> GetBsplineSurfaceFromPoints(const vector<vector<Vector3d>> &pnts, int ordU, int ordV, const GLdouble *const color, GLdouble resol);
