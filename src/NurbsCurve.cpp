#include "NurbsCurve.h"

NurbsCurve::NurbsCurve(const int mord, const ControlPoint* const cp, const int cp_size, const double* const knot,
    const GLdouble* const color, const GLdouble width)
    : BsplineCurve(mord, cp, cp_size, knot, color, width) { }

// �ʒu�x�N�g���擾
Vector3d NurbsCurve::GetPositionVector(const double t) const
{
    Vector3d Q;
    double w = 0.0, W = 0.0;

    // �K�v�Ȓl���v�Z����
    for (int i = 0; i < _ncpnt; i++)
        Q += CalcBsplineFunc(i, _ord, t, &_knot[0]) * (_ctrlp[i] * _ctrlp[i].W); // ����_�𓯎����W�ɕϊ�

    for (int i = 0; i < _ncpnt; i++)
        W += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;

    return Q / W;
}

// �ڐ��x�N�g���擾
Vector3d NurbsCurve::GetFirstDiffVector(const double t) const
{
    Vector3d P, Qt;
    double W = 0.0, Wt = 0.0;

    // �K�v�Ȓl���v�Z����
    P = this->GetPositionVector(t);

    for (int i = 0; i < _ncpnt; i++)
        Qt += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * (_ctrlp[i] * _ctrlp[i].W);

    for (int i = 0; i < _ncpnt; i++)
        W += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;
    for (int i = 0; i < _ncpnt; i++)
        Wt += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i].W;

    return (Qt - Wt * P) / W;
}

// 2�K�����x�N�g���擾
Vector3d NurbsCurve::GetSecondDiffVector(const double t) const
{
    Vector3d P, Pt, Qtt;
    double W = 0.0, Wt = 0.0, Wtt = 0.0;

    // �K�v�Ȓl���v�Z����
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