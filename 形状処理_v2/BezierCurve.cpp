#include "BezierCurve.h"
#include "ControlPoint.h"

BezierCurve::BezierCurve(int mord, ControlPoint* cp, int cp_size, GLdouble* color, GLdouble width, double resol)
{
    _ord = mord;
    _ncpnt = cp_size;
    _min_draw_param = 0.0;
    _max_draw_param = 1.0;

    SetControlPoint(cp, cp_size);
    SetColor(color);
    _width = width;
    _resolution = resol;

    // VBO使う
    _isUseVBO = true;
}

// 事前描画
void BezierCurve::PreDraw()
{
    glColor4dv(_color);
    glLineWidth(_width);

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= 100; i += 1)
    {
        double t = (double)i / 100;
        Vector3d pnt = GetPositionVector(t);

        glVertex3d(pnt);
    }

    glEnd();
}

// 頂点バッファ作成
void BezierCurve::CreateVBO()
{
    vector<Vector3d> pnts;

    // 頂点取得
    for (int i = 0; i <= 100; i += 1)
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
void BezierCurve::DrawVBO()
{
    glColor4dv(_color);
    glLineWidth(_width);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 接線ベクトル描画
void BezierCurve::DrawFirstDiffVectorsInternal()
{
    Vector3d pnt, diff;

    glColor4dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
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
void BezierCurve::DrawSecondDiffVectorsInternal()
{
    Vector3d pnt, diff;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
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
void BezierCurve::DrawNormalVectorsInternal()
{
    Vector3d pnt, normal;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
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
void BezierCurve::DrawCurvatureVectorsInternal()
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = 0; i <= 100; i += 2)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        curv = GetCurvatureVector(t);

        // 曲率半径描画
        glColor4dv(Color::pink);
        glBegin(GL_LINES);
        glVertex3d(pnt);
        glVertex3d(pnt + curv);
        glEnd();

        // 曲率中心描画
        glColor4dv(Color::light_green);
        glBegin(GL_POINTS);
        glVertex3d(pnt + curv);
        glEnd();
    }
}

// 位置ベクトル取得
Vector3d BezierCurve::GetPositionVector(const double t)
{
    Vector3d pnt;

    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBernsteinFunc(i, _ord - 1, t) * _ctrlp[i];

    return pnt;
}

// 接線ベクトル取得
Vector3d BezierCurve::GetFirstDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBernsteinFunc(i, _ord - 1, t) * _ctrlp[i];

    return diff;
}

// 2階微分ベクトル取得
Vector3d BezierCurve::GetSecondDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBernsteinFunc(i, _ord - 1, t) * _ctrlp[i];

    return diff;
}

// 通過点から逆変換して曲線を取得
Curve* BezierCurve::GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width)
{
    vector<ControlPoint> new_cps;
    new_cps.resize(_ncpnt);

    //double dist0 = pnts[0].GetDistance(sample_pnt[1]);
    //double dist1 = sample_pnt[1].GetDistance(sample_pnt[2]);
    //double dist2 = sample_pnt[2].GetDistance(sample_pnt[3]);

    //// 通過点のサンプル位置
    //double sample_t[4] =
    //{
    //    0.0,
    //    dist0 / (dist0 + dist1 + dist2),
    //    (dist0 + dist1) / (dist0 + dist1 + dist2),
    //    1.0,
    //};

    //// 新しい制御点を求める
    //// 基底関数用行列
    //double **B_matrix = new double*[_ncpnt];
    //for (int i = 0; i < _ncpnt; i++)
    //    B_matrix[i] = new double[_ncpnt];

    //// 基底関数行列を作成
    //for (int i = 0; i < _ncpnt; i++)
    //{
    //    for (int j = 0; j < _ncpnt; j++)
    //        B_matrix[i][j] = CalcBernsteinFunc(j, _ord - 1, )
    //}

    return new BezierCurve(_ord, &new_cps[0], _ncpnt, color, width);
}