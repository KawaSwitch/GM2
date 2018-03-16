#include "BsplineCurve.h"
#include "ControlPoint.h"
#include <iterator>

BsplineCurve::BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width, double resol)
{
    _ord = mord;
    _ncpnt = cp_size;
    _nknot = mord + cp_size;

    _min_draw_param = knot[_ord - 1];
    _max_draw_param = knot[_ncpnt];

    SetControlPoint(cp, cp_size);
    SetKnotVector(knot, _nknot);
    SetColor(color);
    _width = width;
    _resolution = resol;

    // VBO使う
    _isUseVBO = true;
}

// ノットベクトル設定
void BsplineCurve::SetKnotVector(double* knot, int size)
{
    if (size <= 0)
        Error::ShowAndExit("ノットベクトル設定失敗", "knot-vector size must be over 0.");

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// ノットベクトルをもとにして点群を取得する
// splitSegCnt: セグメントを何分割するかの回数(デフォルトは1 = 分割しない)
vector<Vector3d> BsplineCurve::GetPointsByKnots(int splitSegCnt)
{
    vector<Vector3d> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (int i = 0; i < _knot.size(); ++i)
    {
        // ノットの階数端の非描画部分or重複は省く
        if ((i > 0 && i < _ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(this->GetPositionVector(_knot[i]));

        // 最後は追加しない
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(this->GetPositionVector(_knot[i] + skip * j));
        }
    }

    return pnts;
}

// 事前描画
void BsplineCurve::PreDraw()
{
    Vector3d pnt;

    glColor3dv(_color);
    glLineWidth(_width);
    glBegin(GL_LINE_STRIP);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        glVertex3d(pnt);
    }

    glEnd();
}

// 頂点バッファ作成
void BsplineCurve::CreateVBO()
{
    vector<Vector3d> pnts;

    // 頂点取得
    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;
        pnts.push_back(GetPositionVector(t));
    }

    _nVertex = (int)pnts.size();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, pnts.size() * 3 * sizeof(double), (GLdouble*)&pnts[0], GL_DYNAMIC_DRAW);
}

// VBOで描画
void BsplineCurve::DrawVBO()
{
    glColor3dv(_color);
    glLineWidth(_width);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 接線ベクトル描画
void BsplineCurve::DrawFirstDiffVectorsInternal()
{
    Vector3d pnt, diff;

    glColor3dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetFirstDiffVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 2階微分ベクトル描画
void BsplineCurve::DrawSecondDiffVectorsInternal()
{
    Vector3d pnt, diff;

    glColor3dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetSecondDiffVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 法線ベクトル描画
void BsplineCurve::DrawNormalVectorsInternal()
{
    Vector3d pnt, normal;

    glColor3dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        normal = GetNormalVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + normal);
    }

    glEnd();
}

// 曲率半径描画
void BsplineCurve::DrawCurvatureVectorsInternal()
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        curv = GetCurvatureVector(t);

        // 曲率半径描画
        glColor3dv(Color::pink);
        glBegin(GL_LINES);
        glVertex3d(pnt);
        glVertex3d(pnt + curv);
        glEnd();

        // 曲率中心描画
        glColor3dv(Color::light_green);
        glBegin(GL_POINTS);
        glVertex3d(pnt + curv);
        glEnd();
    }

    glEnd();
}

// 位置ベクトル取得
Vector3d BsplineCurve::GetPositionVector(const double t)
{
    Vector3d pnt;

    // 標準的
    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return pnt;
}

// 接線ベクトル取得
Vector3d BsplineCurve::GetFirstDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 2階微分ベクトル取得
Vector3d BsplineCurve::GetSecondDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 通過点から逆変換して曲線を取得
// TODO: 時間があったら整理する
Curve* BsplineCurve::GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width)
{
    // TODO: 曲線外から生成

    int passPntsCnt = (int)pnts.size(); // 通過点数
    // _ord = ord;    // 階数はもとの曲線と同じにする
    int new_ncpnt = (passPntsCnt - 1) + (_ord - 1); // 新しい制御点数
    int new_nknot = _ord + new_ncpnt; // 新しいノットベクトルの大きさ

    // ---- ノットベクトルの決定 ----
    
    vector<double> new_knots;
    new_knots.resize(new_nknot);

    vector<double> seg_dist; // セグメント間の距離
    seg_dist.resize(passPntsCnt - 1);

    double minParam = this->GetMinDrawParam();
    double maxParam = this->GetMaxDrawParam();
    double paramRange = fabs(maxParam - minParam);

    // 通過点間の距離の総和
    double sum = 0.0;
    for (int i = 0; i < passPntsCnt - 1; ++i)
    {
        seg_dist[i] = pnts[i].DistanceFrom(pnts[i + 1]);
        sum += seg_dist[i];
    }

    // ノット間隔の割合は通過点間の距離に比例させる
    for (int i = 0; i < new_nknot; i++)
    {
        if (i < _ord) // 最初は階数分重ねる
            new_knots[i] = minParam;
        else if (i < new_nknot - _ord) // 距離に比例
        {
            new_knots[i] = new_knots[i - 1] + (seg_dist[i - _ord] / sum) * paramRange;
        }
        else // 最後も階数分重ねる
            new_knots[i] = maxParam;
    }

    // ---------------------------------

    // ---- 新しい制御点を求めるための連立方程式の準備 ----

    // 1. 連立方程式を解く用の通過点ベクトル
        vector<Vector3d> P_array;

        P_array.push_back(pnts[0]);
        P_array.push_back(Vector3d::Zero());
        for (int i = 1; i < passPntsCnt - 1; ++i)
            P_array.push_back(pnts[i]);
        P_array.push_back(Vector3d::Zero());
        P_array.push_back(pnts[passPntsCnt - 1]);

        // それぞれの要素を射影(selectのC++でのいいやり方が思いつかない)
        vector<double> P_array_x, P_array_y, P_array_z;

        for (auto it = P_array.begin(); it != P_array.end(); ++it)
        {
            P_array_x.push_back((*it).X);
            P_array_y.push_back((*it).Y);
            P_array_z.push_back((*it).Z);
        }

    // 2. 基底関数用行列
        // 行列
        double **N_matrix = new double*[new_ncpnt];
        for (int i = 0; i < new_ncpnt; i++)
            N_matrix[i] = new double[new_ncpnt];

        // 零行列に初期化
        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix[i][j] = 0;
        }

        // 基底関数行列を作成
        for (int i = 0; i < new_ncpnt; i++)
        {
            if (i == 0) // 終端条件 前
            {
                // 一番左のみ1
                N_matrix[i][0] = 1;
            }
            else if (i == 1) // 2階微分終端条件 前
            {
                for (int j = i - 1; j < _ord - 1; ++j)
                    N_matrix[i][j] = Calc2DiffBsplineFunc(j, _ord, new_knots[_ord - 1], &new_knots[0]);
            }
            else if (i > 1 && i < new_ncpnt - 2)
            {
                for (int j = i - 1; j < (_ord - 1) + (i - 1); ++j)
                    N_matrix[i][j] = CalcBsplineFunc(j, _ord, new_knots[(_ord - 1) + (i - 1)], &new_knots[0]);
            }
            else if (i == new_ncpnt - 2) // 2階微分終端条件 後
            {
                for (int j = i - 1; j < (_ord - 1) + (i - 1); ++j)
                    N_matrix[i][j] = Calc2DiffBsplineFunc(j, _ord, new_knots[(_ord - 1) + (i - 1)], &new_knots[0]);
            }
            else // 終端条件 後
            {
                // 一番右のみ1
                N_matrix[i][new_ncpnt - 1] = 1;
            }
        }

        // 引数に合わせて1次元化
        vector<double> N_matrix_flat;
        N_matrix_flat.resize(new_ncpnt * new_ncpnt);

        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix_flat[i * new_ncpnt + j] = N_matrix[i][j];
        }

    // ------------------------------------------------

    // ----- 新しい制御点を求める -----

    vector<double> N_matrix_flat_x, N_matrix_flat_y, N_matrix_flat_z;
    copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_x));
    copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_y));
    copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_z));

    vector<ControlPoint> new_cps;
    vector<double> new_cps_X, new_cps_Y, new_cps_Z;

    // 連立方程式を解く
    new_cps_X = LUDecomposition(new_ncpnt, &N_matrix_flat_x[0], &P_array_x[0]);
    new_cps_Y = LUDecomposition(new_ncpnt, &N_matrix_flat_y[0], &P_array_y[0]);
    new_cps_Z = LUDecomposition(new_ncpnt, &N_matrix_flat_z[0], &P_array_z[0]);

    for (int i = 0; i < new_ncpnt; ++i)
        new_cps.push_back(ControlPoint(new_cps_X[i], new_cps_Y[i], new_cps_Z[i]));

    // ---------------------------------

    return new BsplineCurve(_ord, &new_cps[0], new_ncpnt, &new_knots[0], color, width);
}