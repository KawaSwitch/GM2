#pragma once
#include <vector>
#include "Box.h"

// Initialize.cpp
    GLOBAL void Initialize();
    GLOBAL void InitQuaternion();

// Display.cpp
    GLOBAL void ShowButtonDiscription(char* button, char* disc);
    GLOBAL void ConsoleDiscription();
    GLOBAL void SetRotateCenter();
    GLOBAL void ShowRotateCenter(bool isRotating);
    GLOBAL void UpdateLookAtZ(const Box* const box);

// CalcUtil.cpp
    GLOBAL double ToRad(const double degree);
    GLOBAL double ToDeg(const double radian);
    GLOBAL double GetAverage(const vector<double>& array);
    GLOBAL void SolveQuadraticEquation(double a, double b, double c, double* x1, double* x2);
    GLOBAL double CalcBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double CalcBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* knot);
    GLOBAL double Calc1DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot);
    GLOBAL double Calc2DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot);
    GLOBAL constexpr double Binomial(int n, int k);
    GLOBAL constexpr int Factorial(int n);
    GLOBAL Vector3d CalcPolygonNormal(Vector3d v0, Vector3d v1, Vector3d v2);
    GLOBAL void GetNumberOnly(char *dest, const char *src);
    GLOBAL Point3d Transform(const Point3d point, const double matrix[16]);
    GLOBAL void RotateCoord2DAroundCenter(double* const coord_2d, const double* const center, const double rad);
    GLOBAL void RotateCoord2DAroundOrigin(double* const coord_2d, const double rad);
    GLOBAL vector<double> LUDecomposition(const int size, const double* const aMatrix, const double* const b);

// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);
    GLOBAL void DrawUsingDisplayList(int* const displayList, function<void(void)> RegisterDraw);
    GLOBAL void DrawPoints(vector<Vector3d> vec, GLdouble* color, GLdouble pSize);
    GLOBAL void DrawPoints(vector<Point3d> pnt, GLdouble* color, GLdouble pSize);


// NurbsPrimitive.cpp
    //GLOBAL 

// glUtil.cpp
    GLOBAL void glVertex3d(const Vector3d& vec);
    GLOBAL void glVertex3d(const Point3d& pnt);
    GLOBAL void glPointSize(const GLdouble size);
    GLOBAL void glLineWidth(const GLdouble width);
    GLOBAL void glNormal3d(const Vector3d& vec);
    GLOBAL void glLoadMatrixd(const Matrix3d& mat);
    GLOBAL void glMultMatrixd(const Matrix3d& mat);
    GLOBAL void glColor(const double r, const double g, const double b);
    GLOBAL void glColor(const double r, const double g, const double b, const double a);
    GLOBAL void glMaterialfv(GLenum face, GLenum pname, const GLdouble params[4]);
    GLOBAL void GetCurrentViewPort(std::unique_ptr<int>& viewPort);
    GLOBAL void GetCurrentProjectionMatrix(std::unique_ptr<double>& projection);
    GLOBAL void GetCurrentModelviewMatrix(std::unique_ptr<double>& modelview);
    GLOBAL vector<Point3d> GetLocalCoord(int size, const Point3d* const pnt);
    GLOBAL void glVertex(const vector<Vector3d> vec);
    GLOBAL void glVertex(const vector<Point3d> pnt);
    GLOBAL void RotateAt(const double* mat, const Point3d center);

