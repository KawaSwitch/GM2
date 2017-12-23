#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Background.h"
#include "Axis.h"
#include "ControlPoint.h"
#include "BsplineCurve.h"
#include "Test.h"
#include "Picking.h"

NormalAxis* axis; // Ž²
Scene* scene; // ƒV[ƒ“
extern Scene* test_scene;

extern Point3d center;

void Translate(double* r_mat, double dx, double dy, double dz)
{
    r_mat[0 * 4 + 0] += r_mat[0 * 4 + 3] * dx; r_mat[0 * 4 + 1] += r_mat[0 * 4 + 3] * dy; r_mat[0 * 4 + 2] += r_mat[0 * 4 + 3] * dz;
    r_mat[1 * 4 + 0] += r_mat[1 * 4 + 3] * dx; r_mat[1 * 4 + 1] += r_mat[1 * 4 + 3] * dy; r_mat[1 * 4 + 2] += r_mat[1 * 4 + 3] * dz;
    r_mat[2 * 4 + 0] += r_mat[2 * 4 + 3] * dx; r_mat[2 * 4 + 1] += r_mat[2 * 4 + 3] * dy; r_mat[2 * 4 + 2] += r_mat[2 * 4 + 3] * dz;
    r_mat[3 * 4 + 0] += r_mat[3 * 4 + 3] * dx; r_mat[3 * 4 + 1] += r_mat[3 * 4 + 3] * dy; r_mat[3 * 4 + 2] += r_mat[3 * 4 + 3] * dz;
}

void SetMatrix(Matrix3d matrix)
{
    scene_mat = matrix;
}

int _count = 0;

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();

    // ˆÚ“®
    glTranslated(dist_X, -dist_Y, dist_Z);

    // ‰ñ“]
    //glTranslated(center.X, center.Y, center.Z);
    //Translate(rot_mat, dist_X, -dist_Y, dist_Z);
    //glMultMatrixd(rot_mat);

    //Translate(rot_mat, center.X, center.Y, center.Z);
    //glMultMatrixd(rot_mat);

    glMultMatrixd(rot_mat);
    
    //Translate(rot_mat, -center.X, -center.Y, -center.Z);
    //glMultMatrixd(rot_mat);

    //glMultMatrixd(rot_mat);
    //glTranslated(-center.X, -center.Y, -center.Z);
    
    //scene_mat = Matrix3d::CreateIdentity();

    //glMultMatrixd(rot_mat);


    //SetMatrix(scene_mat.Translate(dist_X, -dist_Y, dist_Z));
    //SetMatrix(scene_mat.RotateAt(target, center));

    //auto mat = Matrix3d();

    //mat = mat.RotateAt(target, center);
    //mat = mat.Translate(dist_X, -dist_Y, dist_Z);


    //glMultMatrixd(mat);
    //glLoadMatrixd(scene_mat);

    //printf("‰ñ“]’†S:%f %f %f\n", center.X, center.Y, center.Z);

    //// ‰ñ“]’†S•`‰æ
    //glColor3dv(Color::red);
    //glPointSize(10.0);
    //glBegin(GL_POINTS);
    //glVertex3d(center.X, center.Y, center.Z);
    //glEnd();

    // Šô‰½ƒOƒŠƒbƒh•`‰æ
    DrawGrid(GeoGrid2D());
    // Œ`ó•`‰æ
    scene->Draw();
    // ƒeƒXƒg•`‰æ
    TestDraw();

    glPopMatrix();

    // Ž²•`‰æ
    glPushMatrix();
    glTranslated(-2.0, -2.0, -2.0);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    _count++;
    glutSwapBuffers();
}