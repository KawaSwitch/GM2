#include "BsplineSurface.h"

BsplineSurface::BsplineSurface(
    int u_mord, int v_mord, ControlPoint* cp, 
    int u_cp_size, int v_cp_size, double* u_knot, double* v_knot,
    GLdouble* color, GLdouble width)
{
    _ordU = u_mord;
    _ordV = v_mord;
    _ncpntU = u_cp_size;
    _ncpntV = v_cp_size;
    _nknotU = u_mord + u_cp_size;
    _nknotV = v_mord + v_cp_size;

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetKnotVector(u_knot, _nknotU, _knotU);
    SetKnotVector(v_knot, _nknotV, _knotV);
}

// 事前描画
void BsplineSurface::PreDraw()
{

}

// 頂点バッファ作成
void BsplineSurface::CreateVBO()
{

}

// VBOで描画
void BsplineSurface::DrawVBO()
{

}

// 接線ベクトル描画
void BsplineSurface::DrawFirstDiffVectorsInternal()
{

}

// 位置ベクトル取得
Vector3d BsplineSurface::GetPositionVector(double u, double v)
{
    Vector3d pnt;



    return pnt;
}

// 接線ベクトル取得
Vector3d BsplineSurface::GetFirstDiffVector(double u, double v)
{
    Vector3d diff;



    return diff;
}