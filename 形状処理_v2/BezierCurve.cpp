#include "BezierCurve.h"
#include "ControlPoint.h"

BezierCurve::BezierCurve(int mord, ControlPoint* cp, int cp_size, GLdouble* color, GLdouble width)
{
    _ord = mord;
    _ncpnt = cp_size;

    SetControlPoint(cp, cp_size);
    SetColor(color);

    // VBO使う
    _isUseVBO = true;
}

// 事前描画
void BezierCurve::PreDraw()
{
    glColor3dv(_color);

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
    glColor3dv(_color);

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

    glColor3dv(Color::red);
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
    //glColor3dv(Color::blue);
    //glLineWidth(1.0);
    //glBegin(GL_LINES);

    //for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    //{
    //    double t = (double)i / 100;

    //    Vector3d pnt = GetPositionVector(t);
    //    Vector3d diff = GetSecondDiffVector(t).Normalize();
    //    glVertex3d(pnt);
    //    glVertex3d(pnt + diff);
    //}

    //glEnd();
}

// 位置ベクトル取得
Vector3d BezierCurve::GetPositionVector(double t)
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

    //for (int i = 0; i < _ncpnt; i++)
    //{
    //    double N = Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]);

    //    diff.X += N * _ctrlp[i].X;
    //    diff.Y += N * _ctrlp[i].Y;
    //    diff.Z += N * _ctrlp[i].Z;
    //}

    return diff;
}