#include "Surface.h"
#include "BsplineCurve.h"

// 制御点設定
void Surface::SetControlPoint(const ControlPoint* const cp, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

    //_ctrlp.reserve(size); _ctrlpX.reserve(size);
    //_ctrlpY.reserve(size); _ctrlpZ.reserve(size);
    //_weight.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);

    // 計算用に各成分割り振る
    for (int i = 0; i < _ncpntU; i++)
    {
        for (int j = 0; j < _ncpntV; j++)
        {
            _ctrlpX.emplace_back(_ctrlp[i * _ncpntV + j].X);
            _ctrlpY.emplace_back(_ctrlp[i * _ncpntV + j].Y);
            _ctrlpZ.emplace_back(_ctrlp[i * _ncpntV + j].Z);
            _weight.emplace_back(_ctrlp[i * _ncpntV + j].W);
        }
    }
}

// 描画用曲率ベクトル取得
Vector3d Surface::GetCurvatureVector(const double u, const double v) const
{
    // 主曲率取得
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    // 単位法線ベクトル
    Vector3d e = GetNormalVector(u, v).Normalize();

    // 絶対値が大きい方を返す
    return (fabs(max_kappa) > fabs(min_kappa)) ?
        (1 / max_kappa) * e :
        (1 / min_kappa) * e;
}

// Σ[i=1tok]Σ[j=1toL] Q(i,j)N[i,n](u)N[j,m](v) を計算する
// <!>非有理曲面の位置ベクトル, 有理曲面の分子
Vector3d Surface::CalcVectorWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const
{
    Vector3d retVec;
    double temp[100]; // 計算用

    vector<double> ctrlpX_w, ctrlpY_w, ctrlpZ_w;

    // 同次座標変換を行う
    for (int i = 0; i < _ncpntU * _ncpntV; i++)
    {
        ctrlpX_w.push_back(_ctrlpX[i] * _weight[i]);
        ctrlpY_w.push_back(_ctrlpY[i] * _weight[i]);
        ctrlpZ_w.push_back(_ctrlpZ[i] * _weight[i]);
    }

    // 行列計算は逆から！
    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &ctrlpX_w[0], temp);
    retVec.X = MatrixMultiply(_ncpntU, temp, BF_array_U);

    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &ctrlpY_w[0], temp);
    retVec.Y = MatrixMultiply(_ncpntU, temp, BF_array_U);

    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &ctrlpZ_w[0], temp);
    retVec.Z = MatrixMultiply(_ncpntU, temp, BF_array_U);

    return retVec;
}

// Σ[i=1tok]Σ[j=1toL] w(i,j)N[i,n](u)N[j,m](v) を計算する
// <!>有理曲面の分母
double Surface::CalcWeightWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const
{
    double temp[100]; // 計算用

    // 行列計算は逆から！
    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &_weight[0], temp);
    return MatrixMultiply(_ncpntU, temp, BF_array_U);
}

// 指定した端の曲線の制御点を取得する
vector<ControlPoint> Surface::GetEdgeCurveControlPoint(const SurfaceEdge edge) const
{
    vector<ControlPoint> edge_cp;

    if (edge == SurfaceEdge::U_min)
    {
        for (int i = 0; i <_ncpntU * _ncpntV; i += _ncpntU)
            edge_cp.push_back(_ctrlp[i]);
    }
    else if (edge == SurfaceEdge::U_max)
    {
        for (int i = _ncpntU - 1; i < _ncpntU * _ncpntV; i += _ncpntU)
            edge_cp.push_back(_ctrlp[i]);
    }
    else if (edge == SurfaceEdge::V_min)
    {
        for (int i = 0; i < _ncpntU; i++)
            edge_cp.push_back(_ctrlp[i]);
    }
    else if (edge == SurfaceEdge::V_max)
    {
        for (int i = _ncpntU * (_ncpntV - 1) - 1; i < _ncpntU * _ncpntV; i++)
            edge_cp.push_back(_ctrlp[i]);
    }

    return edge_cp;
}

// 指定パラメータ固定のアイソ曲線を取得する
std::unique_ptr<Curve> Surface::GetIsoCurve(const ParamUV const_param, const double param, const GLdouble* const color, const GLdouble width) const
{
    vector<Vector3d> pnts; // アイソ曲線取得用の参照点群
    const int split = 30;

    if (const_param == ParamUV::U)
    {
        double skipV = (_max_draw_param_V - _min_draw_param_V) / split;

        for (int i = 0; i <= split; ++i)
            pnts.push_back(GetPositionVector(param, _min_draw_param_V + skipV * i));
    }
    else
    {
        double skipU = (_max_draw_param_U - _min_draw_param_U) / split;

        for (int i = 0; i <= split; ++i)
            pnts.push_back(GetPositionVector(_min_draw_param_U + skipU * i, param));
    }

    return GetBsplineCurveFromPoints(pnts, 4, color, width);
}

// 主曲率を取得
void Surface::GetPrincipalCurvatures(const double u, const double v, double* const max_kappa, double* const min_kappa) const
{
    Vector3d e = GetNormalVector(u, v).Normalize(); // 単位法線ベクトル

    // 各自必要な値を計算
    double L = e.Dot(GetSecondDiffVectorUU(u, v));
    double M = e.Dot(GetSecondDiffVectorUV(u, v));
    double N = e.Dot(GetSecondDiffVectorVV(u, v));
    double E = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorU(u, v));
    double F = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorV(u, v));
    double G = GetFirstDiffVectorV(u, v).Dot(GetFirstDiffVectorV(u, v));

    double A = E * G - F * F;
    double B = 2 * F * M - E * N - G * L;
    double C = L * N - M * M;

    // 主曲率κを計算する（Aκ^2 + Bκ + C = 0 の2解）
    double kappa1, kappa2;
    SolveQuadraticEquation(A, B, C, &kappa1, &kappa2);

    // 最大主曲率と最小主曲率を返す
    *max_kappa = (kappa1 > kappa2) ? kappa1 : kappa2;
    *min_kappa = (kappa1 < kappa2) ? kappa1 : kappa2;
}

// 平均曲率取得
double Surface::GetMeanCurvature(const double u, const double v) const
{
    // 主曲率取得
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    return (max_kappa + min_kappa) / 2.0;
}

// ガウス曲率取得
double Surface::GetGaussianCurvature(const double u, const double v) const
{
    // 主曲率取得
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    return max_kappa * min_kappa;
}

// オブジェクト描画
void Surface::Draw() const
{
    if (isUseLight) // 曲面にはライティング処理を施す
        glEnable(GL_LIGHTING);

    // 基底の描画
    Object::Draw();

    glDisable(GL_LIGHTING);
}

// 制御点線描画
void Surface::DrawCPsInternal() const
{
    //glColor3d(1.0, 0.0, 0.0); // 赤
    glColor3dv(_color);
    glPointSize(10.0);
    glLineWidth(1.0);

    // 点群
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < _ctrlp.size(); i++)
        glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
    glEnd();

    // U方向線群
    for (int i = 0; i < _ncpntV; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < _ncpntU; j++)
            glVertex3d(_ctrlp[i * _ncpntU + j].X, _ctrlp[i * _ncpntU + j].Y, _ctrlp[i * _ncpntU + j].Z);
        glEnd();
    }
    // V方向線群
    for (int i = 0; i < _ncpntU; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < _ncpntV; j++)
            glVertex3d(_ctrlp[j * _ncpntU + i].X, _ctrlp[j * _ncpntU + i].Y, _ctrlp[j * _ncpntU + i].Z);
        glEnd();
    }
}

// 描画範囲を各方向split_num個に分割するような位置ベクトルを取得する(pnts[v][u]と並べる)
void Surface::GetPositionVectors(vector<vector<Vector3d>>& pnts, const int U_split_num, const int V_split_num) const
{
    // 点群のクリア
    for (auto& pntRow : pnts)
        pntRow.clear();
    pnts.clear();

    vector<Vector3d> pos;

    // 分割区間を計算
    double skip_U = (fabs(_min_draw_param_U) + fabs(_max_draw_param_U)) / U_split_num;
    double skip_V = (fabs(_min_draw_param_V) + fabs(_max_draw_param_V)) / V_split_num;

    // double型の誤差考慮
    for (double v = _min_draw_param_V; v < _max_draw_param_V + skip_V / 2; v += skip_V)
    {
        for (double u = _min_draw_param_U; u < _max_draw_param_U + skip_U / 2; u += skip_U)
            pos.push_back(GetPositionVector(u, v));

        pnts.push_back(pos);
        pos.clear();
    }
}

// 参照点からの最近点を取得
NearestPointInfoS Surface::GetNearestPointInfoInternal(const Vector3d& ref, const vector<vector<Point3dS>>& startPnts, const NearestSearch search) const
{
    if (search == Project)
    {
        vector<NearestPointInfoS> possiblePnts; // 最近候補点

        // 開始点毎の最近点を取得し候補点とする
        for (const auto& startPntRow : startPnts)
        {
            for (const auto& startPnt : startPntRow)
                possiblePnts.push_back(this->GetNearestPointFromRefByProjectionMethod(ref, startPnt));
        }

        // 候補の中で一番距離の短いものを最近点とする
        NearestPointInfoS nearestPnt(Vector3d(), Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0, 0);
        for (const auto& p : possiblePnts)
        {
            if (p.dist < nearestPnt.dist)
                nearestPnt = p;
        }

        return nearestPnt;
    }
    else if (search == Isoline)
    {
        // 各開始点の内, 対象点との距離が最短のものを開始点とする
        double dist;
        double min_dist = DBL_MAX;
        Point3dS start(Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0, 0);

        for (const auto& startPntRow : startPnts)
        {
            for (const auto& startPnt : startPntRow)
            {
                dist = ref.DistanceFrom((Vector3d)const_cast<Point3dS &>(startPnt));
                if (dist < min_dist)
                {
                    min_dist = dist;
                    start = startPnt;
                }
            }
        }

        // アイソライン法は適切な開始点の選択で1発で最近点が決定する
        return this->GetNearestPointFromRefByIsolineMethod(ref, start);
    }
    else
        throw;
}
// 最近点を取得する(射影法)
// コメント内のe_.は資料の終了条件番号
NearestPointInfoS Surface::GetNearestPointFromRefByProjectionMethod(const Vector3d& ref, const Point3dS& start) const
{
    int count = 0; // ステップ数

    // 初期パラメータ
    double u = start.paramU;
    double v = start.paramV;

    Vector3d p; // 位置ベクトル
    Vector3d pu; // U方向接線ベクトル
    Vector3d pv; // V方向接線ベクトル
    double delta_u; // U方向パラメータ移動量
    double delta_v; // V方向パラメータ移動量

    // 更新
    auto update = [&]()
    {
        // 各ベクトル更新
        p = GetPositionVector(u, v);
        pu = GetFirstDiffVectorU(u, v);
        pv = GetFirstDiffVectorV(u, v);
        // パラメータ移動量更新
        delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.7;
        delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.7;
    };

    update(); // 初期更新

    while (true)
    {
        u += delta_u;
        v += delta_v;

        // パラメータが描画範囲からはみ出す場合
        if (u < _min_draw_param_U || u > _max_draw_param_U ||
            v < _min_draw_param_V || v > _max_draw_param_V)
            return this->GetNearestPointWhenParamOver(ref, u, v);

        update(); // 更新

        // e1. 注目ベクトルとPuが直交 かつ 注目ベクトルとPvが直交
        if (fabs((ref - p).Dot(pu)) < EPS::NEAREST && fabs((ref - p).Dot(pv) < EPS::NEAREST))
            break;
        // e2. 実座標の移動量がU,V方向ともに0
        if (GetPositionVector(u, v).DistanceFrom(GetPositionVector(u - delta_u, v)) < EPS::NEAREST &&
            GetPositionVector(u, v).DistanceFrom(GetPositionVector(u, v - delta_v)) < EPS::NEAREST)
            break;
        // e3. パラメータ移動量がU,V方向ともに0
        if (fabs(delta_u) < EPS::NEAREST && fabs(delta_v) < EPS::NEAREST)
            break;
        // e4. ステップ数上限
        if (++count > EPS::COUNT_MAX)
            break;
        // e5. 曲面上の点
        if (ref.DistanceFrom(p) < EPS::DIST)
            break;
    }

    return NearestPointInfoS(p, ref, u, v);
}
// パラメータがはみ出したときの最近点取得(曲面の端に最近点があるはず)
// u,v方向がともにはみ出す場合もOK
NearestPointInfoS Surface::GetNearestPointWhenParamOver(const Vector3d& ref, const double u, const double v) const
{
    std::unique_ptr<Curve> edge; // 曲面の端
    std::unique_ptr<NearestPointInfoC> nearInfo; // 最近店情報

    // u方向がはみ出る場合
    if (u < _min_draw_param_U || u > _max_draw_param_U)
    {
        if (u < _min_draw_param_U)
        {
            edge = GetEdgeCurve(SurfaceEdge::U_min);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, _min_draw_param_U, nearInfo->param);
        }
        else
        {
            edge = GetEdgeCurve(SurfaceEdge::U_max);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, _max_draw_param_U, nearInfo->param);
        }
    }
    // v方向がはみ出る場合
    else if (v < _min_draw_param_V || v > _max_draw_param_V)
    {
        if (v < _min_draw_param_V)
        {
            edge = GetEdgeCurve(SurfaceEdge::V_min);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, nearInfo->param, _min_draw_param_V);
        }
        else
        {
            edge = GetEdgeCurve(SurfaceEdge::V_max);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, nearInfo->param, _max_draw_param_V);
        }
    }

    throw;
}
// 最近点を取得する(アイソライン法)
// コメント内のe_は資料の終了条件番号
NearestPointInfoS Surface::GetNearestPointFromRefByIsolineMethod(const Vector3d& ref, const Point3dS& start) const
{
    int count = 0; // ステップ数
    std::unique_ptr<Curve> iso; // アイソ曲線
    std::unique_ptr<NearestPointInfoC> nearInfo; // 最近点情報
    double u, v; // 現在のパラメータ
    double end_param; // 終了点のパラメータ
    double dot; // 内積値

    double ori_rangeU = _max_draw_param_U - _min_draw_param_U;
    double ori_rangeV = _max_draw_param_V - _min_draw_param_V;

    // 初期更新
    u = start.paramU;
    v = start.paramV;
    iso = this->GetIsoCurve(ParamUV::V, v);
    dot = (ref - (Vector3d)const_cast<Point3dS &>(start)).Dot(GetFirstDiffVectorU(u, v));
    if (dot > 0)
        end_param = _max_draw_param_U;
    else
        end_param = _min_draw_param_U;

    while (true)
    {
        // 最近点情報取得
        if (count % 2 == 0)
        {
            if (u < end_param)
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, u / ori_rangeU, end_param / ori_rangeU, 8));
            else
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, end_param / ori_rangeU, u / ori_rangeU, 8));
        }
        else
        {
            if (v < end_param)
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, v / ori_rangeV, end_param / ori_rangeV, 8));
            else
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, end_param / ori_rangeV, v / ori_rangeV, 8));
        }
        ++count;

        // U, V方向のアイソ曲線を交互に取る
        if (count % 2 == 0) // U方向
        {
            v = nearInfo->param * ori_rangeV;
            iso = this->GetIsoCurve(ParamUV::V, v);

            dot = (ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorU(u, v));
            if (dot > 0)
                end_param = _max_draw_param_U;
            else
                end_param = _min_draw_param_U;
        }
        else // V方向
        {
            u = nearInfo->param * ori_rangeU;
            iso = this->GetIsoCurve(ParamUV::U, u);

            dot = (ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorV(u, v));
            if (dot > 0)
                end_param = _max_draw_param_V;
            else
                end_param = _min_draw_param_V;
        }

        // e1. ベクトルの内積がU,V方向ともに直交
        if ((fabs((ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorU(u, v))) < EPS::NEAREST) &&
            (fabs((ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorV(u, v))) < EPS::NEAREST))
            break;
        if (count > 1) // 1回目は端が最近点だとV方向に動かないから移動条件は外す
        {
            // e2. 実座標の移動量がU,V方向ともに0
            if (GetPositionVector(u, v).DistanceFrom(GetPositionVector(u - nearInfo->param * ori_rangeU, v)) < EPS::NEAREST &&
                GetPositionVector(u, v).DistanceFrom(GetPositionVector(u, v - nearInfo->param * ori_rangeV)) < EPS::NEAREST)
                break;
            // e3. パラメータ移動量がU,V方向ともに0
            if (fabs(u - nearInfo->param * ori_rangeU) < EPS::NEAREST && fabs(v - nearInfo->param * ori_rangeV) < EPS::NEAREST)
                break;
        }
        // e4. ステップ数上限
        if (count > EPS::COUNT_MAX)
            break;
        // e5. 曲面上の点
        if (ref.DistanceFrom(nearInfo->nearestPnt) < EPS::DIST)
            break;
    }

    return NearestPointInfoS(nearInfo->nearestPnt, ref, u, v);
}
