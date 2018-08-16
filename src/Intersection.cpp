#include "Intersection.h"

// 曲線と曲面ペアセット
void IntersectSolver::SetPair(const std::shared_ptr<Curve> c, const std::shared_ptr<Surface> s)
{
  _curve = c;
  _surface = s;
}

// 交点取得
Vector3d IntersectSolver::GetIntersect()
{
  // あらかじめ干渉ペアを粗く見積もり交点候補の範囲を狭めておく
  this->CalcRoughInterferePair();

  // 絞った範囲から各アルゴリズムを実行する
  if (_algo == Algo::BoxInterfere)
    {
      
    }
  else
    perror("GetIntersect"); // 未実装

  cout << _interferePair.size() << endl;
  return _interferePair[0].first->GetBound().Center();
  
  //return Vector3d();
}

// おおまかにボックスの干渉するオブジェクトペアを取得する
// とりあえずボックス干渉1回のペア
void IntersectSolver::CalcRoughInterferePair(const int c_split, const int s_splitU, const int s_splitV)
{
  // 最初にセットしたオブジェクトに対して1回分
  this->CalcInterferePair(_curve, _surface, c_split, s_splitU, s_splitV, _interferePair);
}

// 曲線と曲面の干渉ペアを取得
void IntersectSolver::CalcInterferePair(const std::shared_ptr<Curve> curve, const std::shared_ptr<Surface> surf, const int c_split, const int s_splitU, const int s_splitV, std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>>& interfere)
{
  // 曲線を分割
  std::vector<std::shared_ptr<Curve>> split_curves;
  curve->GetDevidedCurves(c_split, split_curves);

  // 曲面を分割
  std::vector<std::vector<std::shared_ptr<Surface>>> split_surfs;
  surf->GetDevidedSurfaces(s_splitU, s_splitV, split_surfs, _surface->_color);

  // 干渉ペアを全探索
  for (const auto& c : split_curves)
    {
      for (const auto& us : split_surfs)
	{
	  for (const auto& vs : us)
	    {
	      if (c->GetBound().IsInterfere(vs->GetBound()))
		{
		  interfere.push_back(std::make_pair(c, vs));
		}
	    }
	}
    }
}

// ボックス干渉法で
Vector3d IntersectSolver::GetIntersectByBoxInterfere()
{
  return Vector3d();
}
