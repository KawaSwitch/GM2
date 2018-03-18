#include "GV.h"

// OpenGL‚ÉŠÖ‚·‚éƒ†[ƒeƒBƒŠƒeƒB‚ğ’è‹`‚µ‚Ü‚·
// •`‰æŒn‚ÍDrawUtil.cpp‚Å’è‹`‚µ‚Ä‚­‚¾‚³‚¢

void glVertex3d(const Vector3d& vec) { glVertex3d(vec.X, vec.Y, vec.Z); }
void glVertex3d(const Point3d& pnt) { glVertex3d(pnt.X, pnt.Y, pnt.Z); }
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

void glVertex(const vector<Vector3d> vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
        glVertex3d(*it);
}
void glVertex(const vector<Point3d> pnt)
{
    for (auto it = pnt.begin(); it != pnt.end(); ++it)
        glVertex3d(*it);
}

// ‰ñ“]’†S‚ğw’è‚µ‚Ä‰ñ“]
void RotateAt(const double* mat, const Point3d center)
{
    // Œ´“_‚É–ß‚µ‚Ä‰ñ“]‚·‚é(s—ñ‚ÌŠ|‚¯Z‚Í‹t!)
    glTranslated(center.X, center.Y, center.Z);
    glMultMatrixd(mat);
    glTranslated(-center.X, -center.Y, -center.Z);
}
