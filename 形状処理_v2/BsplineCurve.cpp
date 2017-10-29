#include "BsplineCurve.h"

BsplineCurve::BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width)
{
    _ord = mord;
    _ncpnt = cp_size;
    _nknot = mord + cp_size;

    SetControlPoint(cp, cp_size);
    SetKnotVector(knot, _nknot);
    SetColor(color);

    // VBO使う
    _isUseVBO = true;
}

// 事前描画
void BsplineCurve::PreDraw()
{
    glBegin(GL_LINE_STRIP);

    for (int i = (int)(_knot[0] * 100); i <= (int)(_knot[_nknot - 1] * 100); i++)
    {
        double t = (double)i / 100;

        Vector3d pnt = GetPositionVector(t);
        glVertex3d(pnt.X, pnt.Y, pnt.Z);
    }

    glEnd();
}

// 頂点バッファ作成
void BsplineCurve::CreateVBO()
{
    vector<Vector3d> pnts;

    // 頂点取得
    for (int i = (int)(_knot[0] * 100); i <= (int)(_knot[_nknot - 1] * 100); i++)
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
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 位置ベクトル取得
Vector3d BsplineCurve::GetPositionVector(double t)
{
    Vector3d pnt;

    for (int i = 0; i < _ncpnt; i++)
    {
        double N = CalcBsplineFunc(i, _ord, t, &_knot[0]);

        pnt.X += N * _ctrlp[i].X;
        pnt.Y += N * _ctrlp[i].Y;
        pnt.Z += N * _ctrlp[i].Z;
    }

    return pnt;
}