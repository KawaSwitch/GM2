#include "Surface.h"

// 制御点設定
void Surface::SetControlPoint(ControlPoint* cp, int size)
{
    if (size <= 0)
        Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

    _ctrlp.reserve(size); _ctrlpX.reserve(size);
    _ctrlpY.reserve(size); _ctrlpZ.reserve(size);
    _weight.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);

    // 計算用に転置する(もっと見栄えがいいやり方が欲しい)
    for (int i = 0; i < _ncpntU; i++)
    {
        for (int j = 0; j < _ncpntV; j++)
        {
            _ctrlpX.push_back(_ctrlp[j * _ncpntU + i].X);
            _ctrlpY.push_back(_ctrlp[j * _ncpntU + i].Y);
            _ctrlpZ.push_back(_ctrlp[j * _ncpntU + i].Z);
            _weight.push_back(_ctrlp[j * _ncpntU + i].W);
        }
    }
}

// 描画用曲率ベクトル取得
Vector3d Surface::GetCurvatureVector(double u, double v)
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
Vector3d Surface::CalcVectorWithBasisFunctions(double* BF_array_U, double* BF_array_V)
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
    
    MatrixMultiply(1, _ncpntU, _ncpntV, BF_array_U, &ctrlpX_w[0], temp);
    retVec.X = MatrixMultiply(_ncpntV, temp, BF_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, BF_array_U, &ctrlpY_w[0], temp);
    retVec.Y = MatrixMultiply(_ncpntV, temp, BF_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, BF_array_U, &ctrlpZ_w[0], temp);
    retVec.Z = MatrixMultiply(_ncpntV, temp, BF_array_V);

    return retVec;
}

// Σ[i=1tok]Σ[j=1toL] w(i,j)N[i,n](u)N[j,m](v) を計算する
// <!>有理曲面の分母
double Surface::CalcWeightWithBasisFunctions(double* BF_array_U, double* BF_array_V)
{
    double temp[100]; // 計算用

    MatrixMultiply(1, _ncpntU, _ncpntV, BF_array_U, &_weight[0], temp);
    return MatrixMultiply(_ncpntV, temp, BF_array_V);
}


// 指定した端の曲線の制御点を取得する
vector<ControlPoint> Surface::GetEdgeCurveControlPoint(SurfaceEdge edge)
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

// 主曲率を取得
void Surface::GetPrincipalCurvatures(double u, double v, double* max_kappa, double* min_kappa)
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
double Surface::GetMeanCurvature(double u, double v)
{
    // 主曲率取得
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    return (max_kappa + min_kappa) / 2.0;
}

// ガウス曲率取得
double Surface::GetGaussianCurvature(double u, double v)
{
    // 主曲率取得
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    return max_kappa * min_kappa;
}

// オブジェクト描画
void Surface::Draw()
{
    if (isUseLight) // 曲面にはライティング処理を施す
        glEnable(GL_LIGHTING);

    // 基底の描画
    Object::Draw();

    glDisable(GL_LIGHTING);
}

// 制御点線描画
void Surface::DrawCPsInternal()
{
    glColor3d(1.0, 0.0, 0.0); // 赤
    glPointSize(5.0);
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

// 参照点からの最近点を取得
Vector3d Surface::GetNearestPointFromRef(Vector3d ref)
{
    int count = 0;

    // 許容値
    const double EPS = 10e-6;

    // 初期パラメータ とりあえず中心で
    double u = (_min_draw_param_U + _max_draw_param_U) / 2.0;
    double v = (_min_draw_param_V + _max_draw_param_V) / 2.0;

    // 正射影法(DAP法)
    Vector3d p = GetPositionVector(u, v);
    Vector3d pu = GetFirstDiffVectorU(u, v);
    Vector3d pv = GetFirstDiffVectorV(u, v);

    double delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.5; // 0.7掛けだとおかしいときがある
    double delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.5; // CGS_bspline_surface_1.kjs

    while (fabs(delta_u) > EPS || fabs(delta_v) > EPS)
    {
        u += delta_u;
        v += delta_v;

        // 注目ベクトルとPuが直角 かつ 注目ベクトルとPvが直角
        if (fabs((ref - p).Dot(pu)) < EPS && fabs((ref - p).Dot(pv) < EPS))
            break;

        // u, v方向ともにはみ出る場合も片方の判定だけで十分
        // u方向がはみ出る場合
        if (u < _min_draw_param_U || u > _max_draw_param_U)
        {
            if (u < _min_draw_param_U)
            {
                Curve* edge = GetEdgeCurve(SurfaceEdge::U_min);
                return edge->GetNearestPointFromRef(ref);
            }
            else
            {
                Curve* edge = GetEdgeCurve(SurfaceEdge::U_max);
                return edge->GetNearestPointFromRef(ref);
            }
        }
        // v方向がはみ出る場合
        else if (v < _min_draw_param_V || v > _max_draw_param_V)
        {
            if (v < _min_draw_param_V)
            {
                Curve* edge = GetEdgeCurve(SurfaceEdge::V_min);
                return edge->GetNearestPointFromRef(ref);
            }
            else
            {
                Curve* edge = GetEdgeCurve(SurfaceEdge::V_max);
                return  edge->GetNearestPointFromRef(ref);
            }
        }

        p = GetPositionVector(u, v);
        pu = GetFirstDiffVectorU(u, v);
        pv = GetFirstDiffVectorV(u, v);

        delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.7; // 更新は0.7でも問題なかった
        delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.7; // CGS_bspline_surface_1.kjs

                                                                   // 繰り返し数が十分なら最近点とする
        if (count++ > 1000)
            break;
    }

    return GetPositionVector(u, v);
}