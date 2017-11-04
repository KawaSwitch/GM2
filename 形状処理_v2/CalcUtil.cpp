#include "GV.h"
#include "ControlPoint.h"

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

double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    return Binomial(N, i) * pow(t, i - 1) * pow(1 - t, N - i - 1) * (i - N * t);
}

double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    return Binomial(N, i) * pow(t, i - 2) * pow(1 - t, N - i - 2)
        * (t*t * (N*N - N) + 2 * t*(1 - i*N) + i * (i - 1));
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
        double tmp_left = 0.0 , tmp_right = 0.0;

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            tmp_left = ((t - knot[i]) / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            tmp_right = ((knot[i + M] - t) / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return tmp_left + tmp_right;
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
        double tmp_left = 0.0, tmp_right = 0.0;

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            tmp_left = (1 / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            tmp_right = (1 / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (tmp_left - tmp_right);
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
        double tmp_left = 0.0, tmp_right = 0.0;

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            tmp_left = (1 / (knot[i + M - 1] - knot[i])) * Calc1DiffBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            tmp_right = (1 / (knot[i + M] - knot[i + 1])) * Calc1DiffBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (tmp_left - tmp_right);
    }
}