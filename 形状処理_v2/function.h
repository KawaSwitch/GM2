#pragma once
#include <vector>

// Initialize.cpp
    GLOBAL void Initialize();

// CalcUtil.cpp
    GLOBAL double CalcBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double CalcBsplineFunc(unsigned int i, unsigned int M, double t, double* knot);
    GLOBAL double Calc1DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot);
    GLOBAL double Calc2DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot);
    GLOBAL constexpr double Binomial(int n, int k);
    GLOBAL constexpr int Factorial(int n);


// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);
    GLOBAL void DrawUsingDisplayList(int* const displayList, function<void(void)> RegisterDraw);

// glOverload.cpp
    GLOBAL void glVertex3d(const Vector3d& vec);
    GLOBAL void glPointSize(double size);
    GLOBAL void glLineWidth(double width);