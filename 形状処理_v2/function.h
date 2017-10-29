#pragma once
#include <vector>

// Initialize.cpp
    GLOBAL void Initialize();

// CalcUtil.cpp
    GLOBAL void CalcBsplineFunc(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf);

// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);