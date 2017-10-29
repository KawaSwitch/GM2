#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Axis.h"

NormalAxis* axis; // 軸
Scene* scene; // シーン

double a[10] = { -3, -2, -1, 0, 1, 2, 3, 4, 5, 6 };
double b[10] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 3 };
double c[10] = { 0, 0, 0, 0, 1, 1, 3, 3, 3, 3 };

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    glPushMatrix();
    glTranslated(dist_X, -dist_Y, dist_Z);
    glMultMatrixd(rot_mat);

    //DrawBsplineFunc(4, 6, 10, a, -3.0, 6.0);
    DrawBsplineFunc(4, 6, 10, b, 0.0, 3.0);
    //DrawBsplineFunc(4, 6, 10, c, 0.0, 3.0);

    // 幾何グリッド描画
    DrawGrid(GeoGrid2D());

    glPopMatrix();

    // 軸描画
    glPushMatrix();
    glTranslated(-2.0, -2.0, -2.0);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    glutSwapBuffers();
}