#include "GV.h"
#include "ControlPoint.h"

// Bスプライン基底関数値を求める
double CalcBsplineFunc(int i, int M, double t, double* knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1])
                return 1.0;
            else
                return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1])
                return 1.0;
            else
                return 0.0;
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
double Calc1DiffBsplineFunc(int i, int M, double t, double* knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1])
                return 1.0;
            else
                return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1])
                return 1.0;
            else
                return 0.0;
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

// Bスプライン基底関数値
void CalcBsplineFunc2(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf)
{
    vector<double> basis; // 基底関数値
    vector<vector<double>> bbf; // 各階数基底関数値バッファ

    // 1階の基底関数値を取得
    for (int i = 0; i < nknot - 1; i++)
    {
        if ((knot[i] <= t) && (t < knot[i + 1]))
            basis.push_back(1.0);
        else
            basis.push_back(0.0);
    }

    bbf.push_back(basis);
    basis.clear();

    double tmp1 = 0.0;
    double tmp2 = 0.0;

    // 2階以降の基底関数値を取得
    for (int M = 2; M <= reqord; M++)
    {
        for (int j = 0; j < nknot - M; j++)
        {
            tmp1 = tmp2 = 0.0;

            // 左側
            if (knot[j] != knot[j + M - 1])
                tmp1 = (t - knot[j]) / (knot[j + M - 1] - knot[j]) * bbf[M - 1 - 1][j];

            // 右側
            if (knot[j + 1] != knot[j + M])
                tmp2 = (knot[j + M] - t) / (knot[j + M] - knot[j + 1]) * bbf[M - 1 - 1][j + 1];

            // 合計
            basis.push_back(tmp1 + tmp2);
        }

        bbf.push_back(basis);
        basis.clear();
    }

    // return
    for (int i = 0; i < nknot - reqord; i++)
        bf[i] = bbf[reqord - 1][i];
}

// 1階の基底関数値を取得
vector<double> Blending_1ORD(double t, int mord, int ncpnt, int nknot, double* knot)
{
    // パラメータ確認
    if (t < knot[0] || t > knot[nknot - 1])
        Error::ShowAndExit("与えられたパラメータの値は不正です.", "パラメータはノットベクトル区間内に設定してください.");

    vector<double> basis; // 基底関数値

    double buf = 0.0;
    for (int i = 0; i < nknot - 1; i++)
        buf += knot[i + 1] - knot[i];

    // 一様Bスプライン
    if (fabs(buf - nknot) - 1 < EPS::DIST)
    {
        // 1階の基底関数値
        for (int i = 0; i < nknot - 1; i++)
        {
            if (knot[i] <= t && t < knot[i + 1])
                basis.push_back(1.0);
            else
                basis.push_back(0.0);
        }
    }
    // 非一様Bスプライン
    else
    {
        long is = mord - 1;
        long ie = ncpnt;
        long im = (is + ie) / 2;
        while (im != is) {
            if (t < knot[im])  ie = im;
            else                 is = im;
            im = (is + ie) / 2;
        }

        int iord = 1;
        int nj = is;
        int ioff = ncpnt * (iord - 1) + nj;

        for (int i = 0; i < nknot - 1; i++)
        {
            if (i == ioff)
                basis.push_back(1.0);
            else
                basis.push_back(0.0);
        }
    }

    return basis;
}

// Bスプライン基底関数値
void Blending(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf)
{
    vector<double> basis; // 基底関数値
    vector<vector<double>> bbf; // 各階数基底関数値バッファ

                                // 1階の基底関数値を取得
    bbf.push_back(Blending_1ORD(t, mord, ncpnt, nknot, knot));
    basis.clear();

    double tmp1 = 0.0;
    double tmp2 = 0.0;

    // de Boor Cox アルゴリズム
    for (int M = 2; M <= reqord; M++)
    {
        for (int j = 0; j < nknot - M; j++)
        {
            tmp1 = tmp2 = 0.0;

            // 左側
            if (knot[j] != knot[j + M - 1])
                tmp1 = (t - knot[j]) / (knot[j + M - 1] - knot[j]) * bbf[M - 1 - 1][j];

            // 右側
            if (knot[j + 1] != knot[j + M])
                tmp2 = (knot[j + M] - t) / (knot[j + M] - knot[j + 1]) * bbf[M - 1 - 1][j + 1];

            // 合計
            basis.push_back(tmp1 + tmp2);
        }

        bbf.push_back(basis);
        basis.clear();
    }

    // return
    for (int i = 0; i < nknot - reqord; i++)
        bf[i] = bbf[reqord - 1][i];
}