#include "Curve.h"
#include <cfloat>

// 制御点設定
void Curve::SetControlPoint(const ControlPoint* const cp, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

    // リセット
    if (_ctrlp.size() > 0)
      {
	_ctrlp.clear();
	_ctrlp.shrink_to_fit();
      }
    
    _ctrlp.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);
}

// 制御点描画
void Curve::DrawCPsInternal() const
{
    glColor4dv(_color);
    glPointSize(10.0);
    glLineWidth(1.0);

    // 点群
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < _ctrlp.size(); i++)
        glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
    glEnd();

    // 線群
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i < _ctrlp.size(); i++)
        glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
    glEnd();
}

// 曲率ベクトル取得
Vector3d Curve::GetCurvatureVector(const double t) const
{
    double kappa =  // κ = |Pt×Ptt| / |Pt|^3
        (GetFirstDiffVector(t) * GetSecondDiffVector(t)).Length() // |Pt×Ptt|
        / pow(GetFirstDiffVector(t).Length(), 3); // |Pt|^3

    // 法線方向N = (Pt × Ptt) × Pt
    Vector3d direct = (GetFirstDiffVector(t) * GetSecondDiffVector(t)) * GetFirstDiffVector(t);

    return (1 / kappa) * direct.Normalize();
}

// 描画範囲をsplit_num個に分割するような位置ベクトルを取得する
void Curve::GetPositionVectors(vector<Vector3d>& pnts, const int split_num) const
{
    pnts.clear();

    // 分割区間を計算
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split_num;

    // double型の誤差考慮
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        pnts.push_back(GetPositionVector(t));
}

// 他曲線との相違度を計算します
// NOTE: 現在ノット位置の分布に偏りがあると機能しない
double Curve::CalcDifferency(const Curve* const other) const
{
    int checkCnt = 100; // 距離を測る点の数
    double sumDistance = 0.0; // 相違距離の合計

    // 分割区間を計算
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / checkCnt;

    // double型の誤差考慮
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        sumDistance += this->GetPositionVector(t).DistanceFrom(other->GetPositionVector(t / this->GetDrawParamRange()));

    // 相違距離の平均を返す
    return sumDistance / (double)checkCnt;
}
// 他曲線との相違度を計算します
// 最近点平均
double Curve::CalcDifferency2(const Curve* const other) const
{
    int checkCnt = 100; // 距離を測る点の数
    double sumDistance = 0.0; // 相違距離の合計

    // 参照点群を取得
    vector<Vector3d> ref_pnts;
    other->GetPositionVectors(ref_pnts, checkCnt - 1);

    // 最近点取得
    vector<NearestPointInfoC> nearest_pnts;
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(this->GetNearestPointInfoFromRef(ref_pnts[i]));

    for (const auto& p : nearest_pnts)
        sumDistance += p.dist;

    // 相違距離の平均を返す
    return sumDistance / (double)checkCnt;
}

// 2曲線で一番遠ざかる距離を計算する
double Curve::CalcFarthestDistant(const Curve* const other) const
{
    int checkCnt = 100; // 距離を測る点の数
    double farthestDist = -DBL_MAX; // 2曲線で一番遠い距離

    // 参照点群を取得
    vector<Vector3d> ref_pnts;
   other->GetPositionVectors(ref_pnts, checkCnt - 1);

    // 最近点取得
    vector<NearestPointInfoC> nearest_pnts;
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(this->GetNearestPointInfoFromRef(ref_pnts[i]));

    for (const auto& p : nearest_pnts)
    {
        if (p.dist > farthestDist)
            farthestDist = p.dist;
    }

    return farthestDist;
}

// 曲線長を取得します
// split : 分割数
double Curve::GetLength(int split) const
{
    double dist = 0.0; // 曲線長

    // 分割区間を計算
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split;

    // double型の誤差考慮
    for (double t = _min_draw_param; t < _max_draw_param - skip - skip / 2; t += skip)
        dist += this->GetPositionVector(t).DistanceFrom(this->GetPositionVector(t + skip));

    return dist;
}

// 参照点からの最近点情報を2分探索で取得します
// ref : 参照点, startPnts : 2分探索開始点位置情報
NearestPointInfoC Curve::GetNearestPointInfoInternal(const Vector3d& ref, const vector<Point3dC>& startPnts) const
{
    vector<NearestPointInfoC> possiblePnts; // 最近候補点
    double left, right; // 2分探索用パラメータ

    // 開始点範囲毎の最近点を取得し候補点とする
    for (size_t i = 0, s = startPnts.size(); i < s; ++i)
    {
        left = startPnts[(i == 0) ? i : i - 1].param;
        right = startPnts[(i == s - 1) ? i : i + 1].param;

        possiblePnts.push_back(GetSectionNearestPointInfoByBinary(ref, left, right));
    }

    // 候補の中で一番距離の短いものを最近点とする
    NearestPointInfoC nearestPnt(Vector3d(), Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0);
    for (const auto& p : possiblePnts)
    {
        if (p.dist < nearestPnt.dist)
            nearestPnt = p;
    }

    return nearestPnt;
}

// 区間内最近点取得(2分探索)
// コメント内のe_は資料の終了条件番号
NearestPointInfoC Curve::GetSectionNearestPointInfoByBinary(const Vector3d& ref, const double ini_left, const double ini_right) const
{
    Vector3d pnt, vec_ref_pnt, tan;
    double left, right, middle; // 2分探索用パラメータ
    double dot; // 内積値
    int count = 0; // 2分探索ステップ数

    left = ini_left; right = ini_right;

    auto update = [&]()
    {
        middle = (left + right) / 2;
        pnt = GetPositionVector(middle);
        tan = GetFirstDiffVector(middle);
        vec_ref_pnt = pnt - ref;
        dot = tan.Dot(vec_ref_pnt); // 内積値
    };

    // 初期更新
    update();

    while (left <= right)
    {
        // e5. 参照点が対象曲線の分割線上にある(中央座標と参照点が重なる)
        if (fabs(pnt.DistanceFrom(ref)) < EPS::DIST_SQRT)
            break;

        // e2. パラメータの移動量0
        if (fabs(left - middle) < EPS::NEAREST || fabs(right - middle) < EPS::NEAREST)
            break;
        // e3. 実座標の移動量0
        if (fabs(GetPositionVector(left).DistanceFrom(GetPositionVector(middle))) < EPS::NEAREST ||
            fabs(GetPositionVector(right).DistanceFrom(GetPositionVector(middle))) < EPS::NEAREST)
            break;

        // e1. ベクトルの内積が0
        if (-EPS::NEAREST < dot && dot < EPS::NEAREST)
        {
            // 十分な精度なので見つかったことにする
            break;
        }
        else if (dot >= EPS::NEAREST)
        {
            // 右端更新
            right = middle;
        }
        else if (dot <= -EPS::NEAREST)
        {
            // 左端更新
            left = middle;
        }

        // 各値更新
        update();

        // e4. ステップ数上限に達したらその時点の点を返す
        if (++count > EPS::COUNT_MAX)
            break;
    }

    return NearestPointInfoC(pnt, ref, middle);
}
// 区間内最近点取得(2分探索)
NearestPointInfoC Curve::GetSectionNearestPointInfoByBinary(const Vector3d& ref, double ini_left, double ini_right, int split) const
{
    vector<NearestPointInfoC> possiblePnts; // 最近候補点
    double skip = (ini_right - ini_left) / split;

    // パラメータ範囲を分割して最近点を取得する
    if (fabs(ini_left - ini_right) < EPS::DIST)
    {
        // 左値と右値が等しい場合
        possiblePnts.push_back(GetSectionNearestPointInfoByBinary(ref, ini_left, ini_right));
    }
    else
    {
        for (double param = ini_left; param < ini_right - skip / 2; param += skip)
            possiblePnts.push_back(GetSectionNearestPointInfoByBinary(ref, param, param + skip));
    }

    // 候補の中で一番距離の短いものを最近点とする
    NearestPointInfoC nearestPnt(Vector3d(), Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0);
    for (const auto& p : possiblePnts)
    {
        if (p.dist < nearestPnt.dist)
            nearestPnt = p;
    }

    return nearestPnt;
}
