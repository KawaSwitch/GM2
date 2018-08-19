#include "NurbsSurface.h"
#include "NurbsCurve.h"

// Bsplineと同じ
NurbsSurface::NurbsSurface(
    const int u_mord, const int v_mord,
    const ControlPoint *const cp, const int u_cp_size, const int v_cp_size,
    const double *const u_knot, const double *const v_knot,
    const GLdouble *const color, const GLdouble resol)
    : BsplineSurface(u_mord, v_mord, cp, u_cp_size, v_cp_size, u_knot, v_knot, color, resol) {}

// 指定した端の曲線を取得する
std::unique_ptr<Curve> NurbsSurface::GetEdgeCurve(const SurfaceEdge edge) const
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordV : _ordU;
    vector<double> edge_knot = (edge == U_min || edge == U_max) ? _knotV : _knotU;

    return std::unique_ptr<Curve>(new NurbsCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), &edge_knot[0], Color::red, _mesh_width));
}

// 位置ベクトル取得
Vector3d NurbsSurface::GetPositionVector(const double u, const double v) const
{
    // 基底関数配列(行列計算用)
    double *N_array_U = new double[_ncpntU];
    double *N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);

    // 位置ベクトル算出に必要な値を求める
    Vector3d Q = CalcVectorWithBasisFunctions(N_array_U, N_array_V);
    double W = CalcWeightWithBasisFunctions(N_array_U, N_array_V);

    delete[](delete[] N_array_U, N_array_V);
    return Q / W;
}

// 接線ベクトル取得
Vector3d NurbsSurface::GetFirstDiffVectorU(const double u, const double v) const
{
    // 基底関数配列(行列計算用)
    double *N_array_U = new double[_ncpntU];
    double *N_array_Ud = new double[_ncpntU];
    double *N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntU; i++)
        N_array_Ud[i] = Calc1DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);

    // U方向接線ベクトル算出に必要な値を求める
    Vector3d P = this->GetPositionVector(u, v);
    Vector3d Qu = CalcVectorWithBasisFunctions(N_array_Ud, N_array_V);

    double W = CalcWeightWithBasisFunctions(N_array_U, N_array_V);
    double Wu = CalcWeightWithBasisFunctions(N_array_Ud, N_array_V);

    delete[](delete[](delete[] N_array_U, N_array_Ud), N_array_V);
    return (Qu - Wu * P) / W;
}
Vector3d NurbsSurface::GetFirstDiffVectorV(const double u, const double v) const
{
    // 基底関数配列(行列計算用)
    double *N_array_U = new double[_ncpntU];
    double *N_array_V = new double[_ncpntV];
    double *N_array_Vd = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_Vd[i] = Calc1DiffBsplineFunc(i, _ordV, v, &_knotV[0]);

    // V方向接線ベクトル算出に必要な値を求める
    Vector3d P = this->GetPositionVector(u, v);
    Vector3d Qv = CalcVectorWithBasisFunctions(N_array_U, N_array_Vd);

    double W = CalcWeightWithBasisFunctions(N_array_U, N_array_V);
    double Wv = CalcWeightWithBasisFunctions(N_array_U, N_array_Vd);

    delete[](delete[](delete[] N_array_U, N_array_V), N_array_Vd);
    return (Qv - Wv * P) / W;
}

// 2階微分ベクトル取得
Vector3d NurbsSurface::GetSecondDiffVectorUU(const double u, const double v) const
{
    // 基底関数配列(行列計算用)
    double *N_array_U = new double[_ncpntU];
    double *N_array_Ud = new double[_ncpntU];
    double *N_array_Udd = new double[_ncpntU];
    double *N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntU; i++)
        N_array_Ud[i] = Calc1DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntU; i++)
        N_array_Udd[i] = Calc2DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);

    // V方向2階微分ベクトル算出に必要な値を求める
    Vector3d P = this->GetPositionVector(u, v);
    Vector3d Pu = this->GetFirstDiffVectorU(u, v);

    Vector3d Quu = CalcVectorWithBasisFunctions(N_array_Udd, N_array_V);

    double W = CalcWeightWithBasisFunctions(N_array_U, N_array_V);
    double Wu = CalcWeightWithBasisFunctions(N_array_Ud, N_array_V);
    double Wuu = CalcWeightWithBasisFunctions(N_array_Udd, N_array_V);

    delete[](delete[](delete[](delete[] N_array_U, N_array_Ud), N_array_Udd), N_array_V);
    return (Quu - Wuu * P - 2 * Wu * Pu) / W;
}
Vector3d NurbsSurface::GetSecondDiffVectorUV(const double u, const double v) const
{
    // 基底関数配列(行列計算用)
    double *N_array_U = new double[_ncpntU];
    double *N_array_Ud = new double[_ncpntU];
    double *N_array_V = new double[_ncpntV];
    double *N_array_Vd = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntU; i++)
        N_array_Ud[i] = Calc1DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_Vd[i] = Calc1DiffBsplineFunc(i, _ordV, v, &_knotV[0]);

    // V方向2階微分ベクトル算出に必要な値を求める
    Vector3d P = this->GetPositionVector(u, v);
    Vector3d Pu = this->GetFirstDiffVectorU(u, v);
    Vector3d Pv = this->GetFirstDiffVectorV(u, v);

    Vector3d Quv = CalcVectorWithBasisFunctions(N_array_Ud, N_array_Vd);

    double W = CalcWeightWithBasisFunctions(N_array_U, N_array_V);
    double Wu = CalcWeightWithBasisFunctions(N_array_Ud, N_array_V);
    double Wv = CalcWeightWithBasisFunctions(N_array_U, N_array_Vd);
    double Wuv = CalcWeightWithBasisFunctions(N_array_Ud, N_array_Vd);

    delete[](delete[](delete[](delete[] N_array_U, N_array_Ud), N_array_V), N_array_Vd);
    return (Quv - Wu * Pv - Wv * Pu - Wuv * P) / W;
}
Vector3d NurbsSurface::GetSecondDiffVectorVV(const double u, const double v) const
{
    // 基底関数配列(行列計算用)
    double *N_array_U = new double[_ncpntU];
    double *N_array_V = new double[_ncpntV];
    double *N_array_Vd = new double[_ncpntV];
    double *N_array_Vdd = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_Vd[i] = Calc1DiffBsplineFunc(i, _ordV, v, &_knotV[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_Vdd[i] = Calc2DiffBsplineFunc(i, _ordV, v, &_knotV[0]);

    // V方向2階微分ベクトル算出に必要な値を求める
    Vector3d P = this->GetPositionVector(u, v);
    Vector3d Pv = this->GetFirstDiffVectorV(u, v);

    Vector3d Qvv = CalcVectorWithBasisFunctions(N_array_U, N_array_Vdd);

    double W = CalcWeightWithBasisFunctions(N_array_U, N_array_V);
    double Wv = CalcWeightWithBasisFunctions(N_array_U, N_array_Vd);
    double Wvv = CalcWeightWithBasisFunctions(N_array_U, N_array_Vdd);

    delete[](delete[](delete[](delete[] N_array_U, N_array_V), N_array_Vd), N_array_Vdd);
    return (Qvv - Wvv * P - 2 * Wv * Pv) / W;
}
