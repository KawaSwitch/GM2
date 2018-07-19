#pragma once

#include <functional>
#include "glUtil.h"

void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);
void DrawUsingDisplayList(int* const displayList, std::function<void(void)> RegisterDraw);
void DrawPoints(std::vector<Vector3d> vec, GLdouble* color, GLdouble pSize);
void DrawPoints(std::vector<Vector3d> pnt, GLdouble* color, GLdouble pSize);