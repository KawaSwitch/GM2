#include "Intersection.h"
#include <iostream>
#include <ctime>

extern Scene *test_scene;

// 曲線と曲面ペアセット
void IntersectSolver::SetPair(const std::shared_ptr<Curve> c, const std::shared_ptr<Surface> s)
{
  _curve = c;
  _surface = s;
}

// 交点取得
vector<Vector3d> IntersectSolver::GetIntersects()
{
  vector<Vector3d> intersects;

  // 時間計測
  clock_t start = clock();
  {
    // あらかじめ干渉ペアを粗く見積もり交点候補の範囲を狭めておく
    this->CalcRoughInterferePair();

    // 絞った範囲から各アルゴリズムを実行する
    if (_algo == Algo::BoxInterfere)
    {
      // ボックス干渉法
      intersects = GetIntersectsByBoxInterfere();
    }
    else
      perror("GetIntersect"); // 未実装
  }
  clock_t end = clock();
  double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
  cout << "交点取得時間: " << elapsed << "ミリ秒" << endl;

  for (unsigned i = 0; i < intersects.size(); ++i)
  {
    cout << "交点[" << i + 1 << "]: (" << intersects[i].X << ", "
         << intersects[i].Y << ", "
         << intersects[i].Z << ")" << endl;
  }

  return intersects;
}
// 1交点取得
Vector3d IntersectSolver::GetIntersect()
{
	Vector3d intersects;

	// 時間計測
	clock_t start = clock();
	{
		// あらかじめ干渉ペアを粗く見積もり交点候補の範囲を狭めておく
		this->CalcRoughInterferePair();

		// 絞った範囲から各アルゴリズムを実行する
		if (_algo == Algo::BoxInterfere)
		{
			// ボックス干渉法
			intersects = GetIntersectByBoxInterfere();
		}
		else
			perror("GetIntersect"); // 未実装
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "交点取得時間: " << elapsed << "ミリ秒" << endl;

	cout << "交点: (" 
		<< intersects.X << ", "
		<< intersects.Y << ", "
		<< intersects.Z << ")" << endl;

	return intersects;
}

// おおまかにボックスの干渉するオブジェクトペアを取得する
// とりあえずボックス干渉1回のペア(TODO: より細かな交点群を取得する場合はもっと)
void IntersectSolver::CalcRoughInterferePair(const int c_split, const int s_splitU, const int s_splitV)
{
  // 最初にセットしたオブジェクトに対して1回分
  this->CalcInterferePair(_curve, _surface, c_split, s_splitU, s_splitV, _interferePair);
}

// ボックス干渉法で交点を1点取得(資料のアルゴリズム)
// 交点が見つからなければ強制終了
Vector3d IntersectSolver::GetIntersectByBoxInterfere(const int c_split, const int s_splitU, const int s_splitV)
{
  // 各回数ごとの干渉ペア
  std::vector<std::vector<
      std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>>>
      interferePairs;
  int count = 0;      // 回数(ステージ数)
  Vector3d intersect; // 交点
  bool found = false; // 交点が見つかったか

  // 干渉ペアを更新する
  auto updateInterferePairs = [&](std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> curPairs) -> void {
    std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> stagePairs;

    for (const auto &pair : curPairs)
    {
      vector<Box> pairBoxes{
          pair.first->GetBound(), pair.second->GetBound()};

      // 干渉ボックスペアを囲むボックス
      Box pairBox(pairBoxes);

      // 対角線の長さが十分に小さければボックス中心を交点として返却
      if (pairBox.DiagLength() < _eps)
      {
        intersect = pairBox.Center();
        found = true;
      }

      // 再分割し, 干渉ボックスを探索
      std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> pairs;
      this->CalcInterferePair(pair.first, pair.second, c_split, s_splitU, s_splitV, pairs);
      stagePairs.insert(stagePairs.end(), pairs.begin(), pairs.end());
    }

    interferePairs.push_back(stagePairs);
  };

  while (count < EPS::COUNT_MAX)
  {
    if (count == 0) // 最初
      updateInterferePairs(_interferePair);
    else
      updateInterferePairs(interferePairs[count - 1]);

    if (found)
      return intersect;

    ++count;
  }

  cout << "交点は見つかりませんでした" << endl;
  throw;
}

// ボックス干渉法で交点を複数点取得(なお要改良)
std::vector<Vector3d> IntersectSolver::GetIntersectsByBoxInterfere(const int c_split, const int s_splitU, const int s_splitV)
{
  // 各回数ごとの干渉ペア
  std::vector<std::vector<
      std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>>>
      interferePairs;
  int count = 0;               // 回数(ステージ数)
  vector<Vector3d> intersects; // 交点群

  // 干渉ペアを更新する
  auto updateInterferePairs = [&](std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> curPairs) -> void {
    std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> stagePairs;

    for (const auto &pair : curPairs)
    {
      vector<Box> pairBoxes{
          pair.first->GetBound(), pair.second->GetBound()};

      // 干渉ボックスペアを囲むボックス
      Box pairBox(pairBoxes);

      // 対角線の長さが十分に小さければボックス中心を交点として格納
      if (pairBox.DiagLength() < _eps)
        intersects.push_back(pairBox.Center());

      // 再分割し, 干渉ボックスを探索
      std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> pairs;
      this->CalcInterferePair(pair.first, pair.second, c_split, s_splitU, s_splitV, pairs);
      stagePairs.insert(stagePairs.end(), pairs.begin(), pairs.end());
    }

    interferePairs.push_back(stagePairs);
  };

  while (count < EPS::COUNT_MAX * 100)
  {
    if (count == 0) // 最初
      updateInterferePairs(_interferePair);
    else
      updateInterferePairs(interferePairs[count - 1]);

    ++count;

    // 干渉ボックスがなくなったら終了
    if (interferePairs[count - 1].size() == 0)
    {
      cout << "count: " << count + 1 << endl; // 範囲狭める用のも含めて+1
      DrawInterferePairs(interferePairs);     // 干渉ボックス描画

      return intersects;
    }
  }

  cout << "交点は見つかりませんでした" << endl;
  return intersects;
}

// 曲線と曲面の干渉ペアを取得
void IntersectSolver::CalcInterferePair(const std::shared_ptr<Curve> curve, const std::shared_ptr<Surface> surf, const int c_split, const int s_splitU, const int s_splitV, std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>> &interfere)
{
  // 曲線を分割
  std::vector<std::shared_ptr<Curve>> split_curves;
  if (curve->GetBound().DiagLength() < _eps / 10) // 部分曲線が十分に小さい場合は再分割を行わない
    //curve->GetDevidedCurves(1, split_curves); // 1分割 = もとの曲線
    return;
  else
    curve->GetDevidedCurves(c_split, split_curves);

  // 曲面を分割
  std::vector<std::vector<std::shared_ptr<Surface>>> split_surfs;
  if (surf->GetBound().DiagLength() < _eps / 10) // 部分曲面が十分に小さい場合は再分割を行わない
    //surf->GetDevidedSurfaces(1, 1, split_surfs, _surface->_color);
    return;
  else
    surf->GetDevidedSurfaces(s_splitU, s_splitV, split_surfs, _surface->_color);

  // 干渉ペアを全探索
  for (const auto &c : split_curves)
  {
    for (const auto &us : split_surfs)
    {
      for (const auto &vs : us)
      {
        if (c->GetBound().IsInterfere(vs->GetBound()))
        {
          interfere.push_back(std::make_pair(c, vs));
        }
      }
    }
  }
}

// 干渉ペアをすべて描画する
void IntersectSolver::DrawInterferePairs(std::vector<std::vector<std::pair<std::shared_ptr<Curve>, std::shared_ptr<Surface>>>> interferePairs)
{
  GLdouble curve_color[4], surf_color[4];
  //srand((unsigned)time(NULL)); // 色ランダム変化用

  // 初回の範囲狭める用
  {
    Color::GetRandomColor(curve_color);
	Color::GetRandomColor(surf_color);

    for (auto &pairs : _interferePair)
    {
      pairs.first->SetColor(curve_color);
      pairs.second->SetColor(surf_color);

      test_scene->AddObject(pairs.first->GetName(), pairs.first);
      test_scene->AddObject(pairs.second->GetName(), pairs.second);
    }
  }
  // 残り
  for (const auto &stage_pairs : interferePairs)
  {
    Color::GetRandomColor(curve_color);
    Color::GetRandomColor(surf_color);

    for (auto &pairs : stage_pairs)
    {
      pairs.first->SetColor(curve_color);
      pairs.second->SetColor(surf_color);

      test_scene->AddObject(pairs.first->GetName(), pairs.first);
      test_scene->AddObject(pairs.second->GetName(), pairs.second);
    }
  }
}
