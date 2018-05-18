#pragma once

#include <vector>
#include "Vector.h"
#include "Point3d.h"


double ToRad(const double degree);
double ToDeg(const double radian);
double GetAverage(const std::vector<double>& array);
void SolveQuadraticEquation(double a, double b, double c, double* x1, double* x2);

double CalcBernsteinFunc(unsigned int i, unsigned int N, double t);
double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
double CalcBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* knot);
double Calc1DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot);
double Calc2DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot);
constexpr double Binomial(int n, int k);
constexpr int Factorial(int n);
Vector3d CalcPolygonNormal(Vector3d v0, Vector3d v1, Vector3d v2);
void RotateCoord2DAroundCenter(double* const coord_2d, const double* const center, const double rad);
void RotateCoord2DAroundOrigin(double* const coord_2d, const double rad);
std::vector<double> LUDecomposition(const int size, const double* const aMatrix, const double* const b);