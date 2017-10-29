#pragma once
#include <vector>

// Initialize.cpp
    GLOBAL void Initialize();

// CalcUtil.cpp
    GLOBAL double CalcBsplineFunc(int i, int M, double t, double* knot);
    GLOBAL void CalcBsplineFunc2(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf);

// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);