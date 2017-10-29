#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Axis.h"

NormalAxis* axis; // Ž²

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Šô‰½ƒOƒŠƒbƒh•`‰æ
    glPushMatrix();
    glTranslated(dist_X, -dist_Y, dist_Z);
    //glMultMatrixd(rot_mat);
    DrawGrid(GeoGrid2D());
    glColor3d(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex3d(1.0, 1.0, 0.0);
    glEnd();
    glPopMatrix();

    // Ž²•`‰æ
    glPushMatrix();
    glTranslated(-2.0, -2.0, -2.0);
    //glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    glutSwapBuffers();
}