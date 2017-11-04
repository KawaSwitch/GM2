#include "GV.h"

void glVertex3d(const Vector3d& vec) { glVertex3d(vec.X, vec.Y, vec.Z); }
void glPointSize(const double size) { glPointSize((GLfloat)size); }
void glLineWidth(const double width) { glLineWidth((GLfloat)width); }
void glNormal3d(const Vector3d& vec) { glNormal3d(vec.X, vec.Y, vec.Z); }
void glMaterialfv(GLenum face, GLenum pname, const GLdouble params[4])
{
    GLfloat color[4] = { (GLfloat)params[0], (GLfloat)params[1], (GLfloat)params[2], (GLfloat)params[3] };
    glMaterialfv(face, pname, color);
}