#pragma once

#include <vector>
#include "Vector.h"
#include "ControlPoint.h"
using std::vector;

double CalcBernsteinFunc(unsigned int i, unsigned int N, double t);
double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
double CalcBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double *knot);
double Calc1DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double *const knot);
double Calc2DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double *const knot);

bool IsPassingPntsSetCorrectly(const vector<vector<Vector3d>> &pnts);
void CalcKnotVectorByPassingPnts(const vector<Vector3d> &pnts, const int ord, vector<double> *knot);
void NormalizeKnotVector(const vector<vector<double>> &knot_array, const int ord, vector<double> *knot_normal);
void CalcKnotVectorByPassingPnts(const vector<vector<Vector3d>> &pnts, const int ordU, const int ordV, vector<double> *knotU, vector<double> *knotV);
void CalcControlPointsByPassingPnts(const vector<Vector3d> &pnts, const int ord, const vector<double> &knot, vector<ControlPoint> *ctrlp);

void CalcKnotsForAddingKnot(const double t, const vector<double> &knot, unsigned &insert, vector<double> &new_knot);
void CalcControlPointsForAddingKnot(const double t, const int insert, const int ord, const vector<double> &knot, const vector<ControlPoint> &ctrlp, vector<ControlPoint> &new_ctrlp);
void AdjustKnotVector(vector<double> &knot, int ord, double min, double max);
