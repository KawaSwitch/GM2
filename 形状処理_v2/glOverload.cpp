#include "GV.h"

void glVertex3d(const Vector3d& vec) { glVertex3d(vec.X, vec.Y, vec.Z); }
void glPointSize(double size) { glPointSize((GLfloat)size); }
void glLineWidth(double width) { glLineWidth((GLfloat)width); }