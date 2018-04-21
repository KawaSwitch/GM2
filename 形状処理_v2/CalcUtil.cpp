#define _USE_MATH_DEFINES

#include <numeric>
#include "CalcUtil.h"
#include "Matrix.h"

// 度数法から弧度法に変換する
double ToRad(const double degree)
{
    constexpr double C = M_PI / 180;
    return degree * C;
}
// 弧度法から度数法に変換する
double ToDeg(const double radian)
{
    constexpr double C = 180 / M_PI;
    return radian * C;
}

// 平均値を取得する
double GetAverage(const std::vector<double>& array)
{
    return std::accumulate(array.begin(), array.end(), 0.0) / array.size();
}

// 2次方程式 ax^2 + bx + c = 0 の 2解x1,x2 を求める
// 虚数解の場合 x1 ± x2i を満たす x1,x2 を求める
void SolveQuadraticEquation(const double a, const double b, const double c, double* const x1, double* const x2)
{
    // 判別式
    double D = b * b - 4 * a * c;

    // 異なる2つの実数解
    if (D > 0)
    {
        *x1 = (-b + sqrt(D)) / (2 * a);
        *x2 = (-b - sqrt(D)) / (2 * a);
    }
    // 重解
    else if (D == 0)
    {
        // 重解のとき x = -(b / 2a)
        *x1 = *x2 = -b / (2 * a);
    }
    // 異なる2つの虚数解
    else
    {
        *x1 = -b / (2 * a); // real part
        *x2 = sqrt(-D) / (2 * a); // imaginary part
    }
}

// 二項係数を求める
constexpr double Binomial(const int n, const int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

// 階乗値を計算する
constexpr int Factorial(const int n)
{
    return (n == 0) ? 1 : n * Factorial(n - 1);
}

// バーンスタイン基底関数を求める
double CalcBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    return Binomial(N, i) * pow(t, i) * pow(1 - t, N - i);
}

// 1階微分用バーンスタイン基底関数を求める
double Calc1DiffBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    return Binomial(N, i) * (i * pow(t, i - 1) * pow(1 - t, N - i) - (N - i) * pow(t, i) * pow(1 - t, N - i - 1));
}

// 2階微分用バーンスタイン基底関数を求める
double Calc2DiffBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    double temp_left = i * ((i - 1) * pow(t, i - 2) * pow(1 - t, N - i)
        - pow(t, i - 1) * (N - i) * pow(1 - t, N - i - 1));

    double temp_right = (N - i) * (i * pow(t, i - 1) * pow(1 - t, N - i - 1)
        - pow(t, i) * (N - i - 1) * pow(1 - t, N - i - 2));

    return Binomial(N, i) * (temp_left - temp_right);
}

// Bスプライン基底関数値を求める
double CalcBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0 , temp_right = 0.0;

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = ((t - knot[i]) / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = ((knot[i + M] - t) / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return temp_left + temp_right;
    }
}

// 1階微分用Bスプライン基底関数値を求める
double Calc1DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0, temp_right = 0.0;

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// 2階微分用Bスプライン基底関数値を求める
double Calc2DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0, temp_right = 0.0;

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * Calc1DiffBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * Calc1DiffBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// 3点から成るポリゴンの単位化済み面法線を取得する
Vector3d CalcPolygonNormal(const Vector3d& v0, const Vector3d& v1, const Vector3d& v2)
{
    Vector3d vec01 = v1 - v0;
    Vector3d vec02 = v2 - v0;

    return (vec01 * vec02).Normalize();
}

// 2D:指定座標中心に指定rad回転させる
void RotateCoord2DAroundCenter(double* const coord_2d, const double* const center, const double rad)
{
    // 2 × 2の回転行列
    double rotate_mat[2 * 2] =
    {
        cos(rad), -sin(rad),
        sin(rad), cos(rad),
    };

    coord_2d[0] -= center[0];
    coord_2d[1] -= center[1];

    double temp[2];

    MatrixMultiply(2, 2, 1, rotate_mat, coord_2d, temp);

    // 結果代入
    coord_2d[0] = temp[0] + center[0];
    coord_2d[1] = temp[1] + center[1];
}

// 2D:原点中心に指定rad回転させる
void RotateCoord2DAroundOrigin(double* const coord_2d, const double rad)
{
    double origin[2] = { 0, 0 }; // 原点
    RotateCoord2DAroundCenter(coord_2d, origin, rad);
}

// LU分解で連立方程式を解く
std::vector<double> LUDecomposition(const int size, const double* const aMatrix, const double* const b)
{
    int N = size; // 解く配列のサイズ

    // aMatrixの値コピーを作成
    double* aMatCopy = new double[N * N];
    for (int i = 0; i < N * N; i++)
        aMatCopy[i] = aMatrix[i];

    // L行列(零行列に初期化)
    double **lMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        lMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            lMatrix[i][j] = 0;
    }

    // U行列(対角要素を1に初期化)
    double **uMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        uMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            uMatrix[i][j] = (i == j) ? 1 : 0;
    }

    // 計算用のバッファ
    double **buffer = new double*[N];
    for (int i = 0; i < N; i++)
        buffer[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            buffer[i][j] = 0;
    }

    // LU分解開始
    for (int i = 0; i < N; i++)
    {
        int n = N - i - 1;

        double l0 = lMatrix[i][i] = aMatCopy[0];

        // l1成分をコピー
        double *l1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatCopy[(j + 1) * N + 0];
        }

        // u1^double成分をコピー
        double *u1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatCopy[0 * N + (j + 1)] / l0;
        }

        // luを求める
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                buffer[j][k] = l1[j] * u1[k];
            }
        }

        // A1を求める
        double **A1 = new double*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
                A1[j][k] = aMatCopy[(j + 1) * N + (k + 1)] - buffer[j][k];
        }

        // A1を新しいaMatrixとして利用する
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatCopy[i * N + j] = A1[i][j];
        }
    }

    // LU行列を使って連立方程式を解く
    double *y = new double[N];
    for (int i = 0; i < N; i++)
    {
        double sum = 0;
        for (int k = 0; k <= i - 1; k++)
        {
            sum += lMatrix[i][k] * y[k];
        }
        y[i] = (b[i] - sum) / lMatrix[i][i];
    }

    std::vector<double> x; // 解ベクトル
    x.resize(N);
    for (int i = N - 1; i >= 0; i--)
    {
        double sum = 0;
        for (int j = i + 1; j <= N - 1; j++)
        {
            sum += uMatrix[i][j] * x[j];
        }
        x[i] = y[i] - sum;
    }

    delete aMatCopy;
    return x;
}