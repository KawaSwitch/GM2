#include "NurbsCurve.h"

NurbsCurve::NurbsCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width)
    : BsplineCurve(mord, cp, cp_size, knot, color, width) { }

// 位置ベクトル取得
Vector3d NurbsCurve::GetPositionVector(double t)
{
    Vector3d Q;
    double w = 0.0, W = 0.0;

    // 必要な値を計算する
    for (int i = 0; i < _ncpnt; i++)
        Q += CalcBsplineFunc(i, _ord, t, &_knot[0]) * (_ctrlp[i] * _ctrlp[i].W); // 制御点を同次座標に変換

    for (int i = 0; i < _ncpnt; i++)
        W += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;

    return Q / W;
}

// 接線ベクトル取得
Vector3d NurbsCurve::GetFirstDiffVector(double t)
{
    Vector3d P, Qt;
    double W = 0.0, Wt = 0.0;

    // 必要な値を計算する
    P = this->GetPositionVector(t);

    for (int i = 0; i < _ncpnt; i++)
        Qt += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * (_ctrlp[i] * _ctrlp[i].W);

    for (int i = 0; i < _ncpnt; i++)
        W += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;
    for (int i = 0; i < _ncpnt; i++)
        Wt += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;

    return (Qt - Wt * P) / W;
}

// 2階微分ベクトル取得
Vector3d NurbsCurve::GetSecondDiffVector(double t)
{
    Vector3d P, Pt, Qtt;
    double W = 0.0, Wt = 0.0, Wtt = 0.0;

    // 必要な値を計算する
    P = this->GetPositionVector(t);
    Pt = this->GetFirstDiffVector(t);

    for (int i = 0; i < _ncpnt; i++)
        Qtt += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * (_ctrlp[i] * _ctrlp[i].W);

    for (int i = 0; i < _ncpnt; i++)
        W += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;
    for (int i = 0; i < _ncpnt; i++)
        Wt += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;
    for (int i = 0; i < _ncpnt; i++)
        Wtt += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;

    return (Qtt - Wtt * P - 2 * Wt * Pt) / W;
}