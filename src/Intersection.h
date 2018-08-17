#pragma once

#include <vector>
#include <utility>
#include <memory>
#include "Object.h"
#include "Curve.h"
#include "Surface.h"
#include "EPS.h"
#include "Box.h"

// 交点取得クラス
// 交線には未対応
class IntersectSolver
{
 public:
  // 交点の求め方
  enum Algo 
  {
    BoxInterfere, // ボックス干渉法
    GeoNewton, // 幾何的ニュートン法
  };
  
 private:
  double _eps; // トレランス
  Algo _algo; // 求交点アルゴリズム

  std::shared_ptr<Curve> _curve;
  std::shared_ptr<Surface> _surface;

  // 最初の干渉ペア
  std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> _interferePair;

  // おおまかなボックス干渉ペア計算
  void CalcRoughInterferePair(int c_split = 10, int s_splitU = 10, int s_splitV = 10);

  // ボックス干渉法で交点を1点取得(複数の交点取得は要改良)
  vector<Vector3d> GetIntersectByBoxInterfere(int c_split = 2, int s_splitU = 2, int s_splitV = 2);

  // 曲線と曲面の干渉ペアを取得
  void CalcInterferePair(std::shared_ptr<Curve> curve, std::shared_ptr<Surface> surf, int c_split, int s_splitU, int s_splitV, std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>>& interfere);

  // 干渉ペアをすべて描画する
  void DrawInterferePairs(std::vector<std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>>> interferePairs);
  
 public:

  // コンストラクタ
  IntersectSolver(double tole = EPS::DIST, Algo algo = Algo::GeoNewton)
    {
      _eps = tole;
      _algo = algo;
    }
  
  // トレランスのセット
  void SetTolerance(double e) { _eps = e; }

  // ペアセット
  // 曲線と曲面
  void SetPair(std::shared_ptr<Curve> c, std::shared_ptr<Surface> s);

  // 交点取得
  std::vector<Vector3d> GetIntersects();
};
