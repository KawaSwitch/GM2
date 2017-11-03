#pragma once
#include <vector>

// Initialize.cpp
    GLOBAL void Initialize();

// CalcUtil.cpp
    GLOBAL double CalcBsplineFunc(int i, int M, double t, double* knot);
    GLOBAL double Calc1DiffBsplineFunc(int i, int M, double t, double* knot);
    GLOBAL void CalcBsplineFunc2(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf);

// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);
    GLOBAL void DrawUsingDisplayList(int* const displayList, function<void(void)> RegisterDraw);

// glOverload.cpp
    GLOBAL void glVertex3d(const Vector3d& vec);

    // 1階の基底関数値を取得
    GLOBAL vector<double> Blending_1ORD(double t, int mord, int ncpnt, int nknot, double* knot);

    // Bスプライン基底関数値
    GLOBAL void Blending(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf);