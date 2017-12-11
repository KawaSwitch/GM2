#include "GV.h"

void glVertex3d(const Vector3d& vec) { glVertex3d(vec.X, vec.Y, vec.Z); }
void glPointSize(const double size) { glPointSize((GLfloat)size); }
void glLineWidth(const double width) { glLineWidth((GLfloat)width); }
void glNormal3d(const Vector3d& vec) { glNormal3d(vec.X, vec.Y, vec.Z); }
void glMultMatrixd(const Matrix3d& mat)
{
    const double mat_arr[16] =
    {
        mat.M11, mat.M12, mat.M13, mat.M14,
        mat.M21, mat.M22, mat.M23, mat.M24,
        mat.M31, mat.M32, mat.M33, mat.M34,
        mat.M41, mat.M42, mat.M43, mat.M44,
    };

    glMultMatrixd(mat_arr);
}
void glLoadMatrixd(const Matrix3d& mat)
{
    const double mat_arr[16] =
    {
        mat.M11, mat.M12, mat.M13, mat.M14,
        mat.M21, mat.M22, mat.M23, mat.M24,
        mat.M31, mat.M32, mat.M33, mat.M34,
        mat.M41, mat.M42, mat.M43, mat.M44,
    };

    glLoadMatrixd(mat_arr);
}
void glMaterialfv(GLenum face, GLenum pname, const GLdouble params[4])
{
    GLfloat color[4] = { (GLfloat)params[0], (GLfloat)params[1], (GLfloat)params[2], (GLfloat)params[3] };
    glMaterialfv(face, pname, color);
}