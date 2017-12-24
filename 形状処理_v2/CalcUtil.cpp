#include "GV.h"
#include "ControlPoint.h"

// 2次方程式 ax^2 + bx + c = 0 の 2解x1,x2 を求める
// 虚数解の場合 x1 ± x2i を満たす x1,x2 を求める
void SolveQuadraticEquation(double a, double b, double c, double* x1, double* x2)
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
constexpr double Binomial(int n, int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

// 階乗値を計算する
constexpr int Factorial(int n)
{
    return (n == 0) ? 1 : n * Factorial(n - 1);
}

// バーンスタイン基底関数を求める
double CalcBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    return Binomial(N, i) * pow(t, i) * pow(1 - t, N - i);
}

// 1階微分用バーンスタイン基底関数を求める
double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    return Binomial(N, i) * (i * pow(t, i - 1) * pow(1 - t, N - i) - (N - i) * pow(t, i) * pow(1 - t, N - i - 1));
}

// 2階微分用バーンスタイン基底関数を求める
double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    double temp_left = i * ((i - 1) * pow(t, i - 2) * pow(1 - t, N - i)
        - pow(t, i - 1) * (N - i) * pow(1 - t, N - i - 1));

    double temp_right = (N - i) * (i * pow(t, i - 1) * pow(1 - t, N - i - 1)
        - pow(t, i) * (N - i - 1) * pow(1 - t, N - i - 2));

    return Binomial(N, i) * (temp_left - temp_right);
}

// Bスプライン基底関数値を求める
double CalcBsplineFunc(unsigned int i, unsigned int M, double t, double* knot)
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
double Calc1DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot)
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
double Calc2DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot)
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

// 文字列の中から数字のみを取り出す
void GetNumberOnly(char *dest, const char *src)
{
    while (*src)
    {
        if ('0' <= *src && *src <= '9') 
            *dest++ = *src;
        
        src++;
        *dest = 0;
    }
}

// 3点から成るポリゴンの単位化済み面法線を取得する
Vector3d CalcPolygonNormal(Vector3d v0, Vector3d v1, Vector3d v2)
{
    Vector3d vec01 = v1 - v0;
    Vector3d vec02 = v2 - v0;

    return (vec01 * vec02).Normalize();
}

// 3次元点を幾何学変換する
Point3d Transform(const Point3d point, const double matrix[16]/* 4 × 4行列 */)
{
    return
    {
        point.X * matrix[0 * 4 + 0] + point.Y * matrix[1 * 4 + 0] + point.Z * matrix[2 * 4 + 0] + matrix[3 * 4 + 0],
        point.X * matrix[0 * 4 + 1] + point.Y * matrix[1 * 4 + 1] + point.Z * matrix[2 * 4 + 1] + matrix[3 * 4 + 1],
        point.X * matrix[0 * 4 + 2] + point.Y * matrix[1 * 4 + 2] + point.Z * matrix[2 * 4 + 2] + matrix[3 * 4 + 2],
        //point.X * matrix[0 * 4 + 0] + point.Y * matrix[0 * 4 + 1] + point.Z * matrix[0 * 4 + 2] + matrix[0 * 4 + 3],
        //point.X * matrix[1 * 4 + 0] + point.Y * matrix[1 * 4 + 1] + point.Z * matrix[1 * 4 + 2] + matrix[1 * 4 + 3],
        //point.X * matrix[2 * 4 + 0] + point.Y * matrix[2 * 4 + 1] + point.Z * matrix[2 * 4 + 2] + matrix[2 * 4 + 3],
    };
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
