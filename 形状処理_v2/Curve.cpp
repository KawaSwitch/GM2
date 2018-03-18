#include "Curve.h"

// 制御点設定
void Curve::SetControlPoint(ControlPoint* cp, int size)
{
    if (size <= 0)
        Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

    _ctrlp.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);
}

// 制御点描画
void Curve::DrawCPsInternal()
{
    //glColor3d(1.0, 0.0, 0.0); // 赤
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
Vector3d Curve::GetCurvatureVector(double t)
{
    double kappa =  // κ = |Pt×Ptt| / |Pt|^3
        (GetFirstDiffVector(t) * GetSecondDiffVector(t)).Length() // |Pt×Ptt|
        / pow(GetFirstDiffVector(t).Length(), 3); // |Pt|^3

    // 法線方向N = (Pt × Ptt) × Pt
    Vector3d direct = (GetFirstDiffVector(t) * GetSecondDiffVector(t)) * GetFirstDiffVector(t);

    return (1 / kappa) * direct.Normalize();
}

// 描画範囲をsplit_num個に分割するような位置ベクトルを取得する
vector<Vector3d> Curve::GetPositionVectors(int split_num)
{
    vector<Vector3d> pos;

    // 分割区間を計算
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split_num;

    // double型の誤差考慮
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        pos.push_back(GetPositionVector(t));

    return pos;
}

// 参照点からの最近点を取得する
Vector3d Curve::GetNearestPointFromRef(Vector3d ref)
{
    const double EPS = 10e-12;
    double left = _min_draw_param;
    double right = _max_draw_param;
    double middle = (left + right) / 2;
    int count = 0;

    Vector3d pnt = GetPositionVector(middle);
    Vector3d ref_pnt = pnt - ref;
    Vector3d tan = GetFirstDiffVector(middle);
    double dot = tan.Dot(ref_pnt); // 内積値

    // 二分探索
    // 端対策のために更新の際EPSを増減させる
    while (left <= right)
    {
        if (-EPS < dot && dot < EPS)
        {
            // 十分な精度なので見つかったことにする
            return pnt;
        }
        else if (dot >= EPS)
        {
            // 右端更新
            right = middle - EPS;
        }
        else if (dot <= -EPS)
        {
            // 左端更新
            left = middle + EPS;
        }

        // 中心更新
        middle = (left + right) / 2.0;

        // 端っこが一番近い
        if (middle < _min_draw_param)
            return GetPositionVector(_min_draw_param);
        if (middle > _max_draw_param)
            return GetPositionVector(_max_draw_param);

        // 再計算
        pnt = GetPositionVector(middle);
        ref_pnt = pnt - ref;
        tan = GetFirstDiffVector(middle);
        dot = tan.Dot(ref_pnt); // 内積値

        count++;
    }

    // 見つかった
    return pnt;
}

// 他曲線との相違度を計算します
double Curve::CalcDifferency(Curve* other)
{
    int checkCnt = 100; // 距離を測る点の数
    double sumDistance = 0.0; // 相違距離の合計
    double minParam[2], maxParam[2]; // 端のパラメータ

    minParam[0] = this->GetMinDrawParam();
    maxParam[0] = this->GetMaxDrawParam();
    minParam[1] = other->GetMinDrawParam();
    maxParam[1] = other->GetMaxDrawParam();

    // 分割区間を計算
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / checkCnt;

    // double型の誤差考慮
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        sumDistance += this->GetPositionVector(t).DistanceFrom(other->GetPositionVector(t));
    // 相違距離の平均を返す
    return sumDistance / (double)checkCnt;
}