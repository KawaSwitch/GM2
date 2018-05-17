#pragma once

#include <gl/freeglut.h>
#include <memory>
#include <vector>
#include "Vector.h"
#include "Point3d.h"

void glVertex3d(const Vector3d& vec);
void glVertex3d(const Vector3d& pnt);
void glPointSize(const GLdouble size);
void glLineWidth(const GLdouble width);
void glNormal3d(const Vector3d& vec);
void glColor(const double r, const double g, const double b);
void glColor(const double r, const double g, const double b, const double a);
void glMaterialfv(GLenum face, GLenum pname, const GLdouble params[4]);
void GetCurrentViewPort(std::unique_ptr<int>& viewPort);
void GetCurrentProjectionMatrix(std::unique_ptr<double>& projection);
void GetCurrentModelviewMatrix(std::unique_ptr<double>& modelview);
std::vector<Vector3d> GetLocalCoord(int size, const Vector3d* const pnt);
void glVertex(const std::vector<Vector3d> vec);
void glVertex(const std::vector<Vector3d> pnt);
void RotateAt(const double* mat, const Vector3d center);