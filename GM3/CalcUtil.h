#pragma once

#include <vector>
#include "Vector.h"
#include "Point3d.h"
#include "CalcGeoModelUtil.h"
#include "struct.h"

double ToRad(const double degree);
double ToDeg(const double radian);
double GetAverage(const std::vector<double> &array);
constexpr double Binomial(int n, int k);
constexpr int Factorial(int n);

Vector3d CalcPolygonNormal(Vector3d v0, Vector3d v1, Vector3d v2);
void RotateCoord2DAroundCenter(double *const coord_2d, const double *const center, double rad);
void RotateCoord2DAroundOrigin(double *const coord_2d, double rad);

void SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2);
std::vector<double> LUDecomposition(int size, const double *const aMatrix, const double *const b);

bool IsNear(Point<double> pntA, Point<double> pntB, double r);