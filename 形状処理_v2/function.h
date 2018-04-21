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


// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);
    GLOBAL void DrawUsingDisplayList(int* const displayList, function<void(void)> RegisterDraw);
    GLOBAL void DrawPoints(vector<Vector3d> vec, GLdouble* color, GLdouble pSize);
    GLOBAL void DrawPoints(vector<Point3d> pnt, GLdouble* color, GLdouble pSize);


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

