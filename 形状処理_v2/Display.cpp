#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Axis.h"

NormalAxis* axis; // Ž²
Scene* scene; // ƒV[ƒ“

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    glPushMatrix();
    glTranslated(dist_X, -dist_Y, dist_Z);
    glMultMatrixd(rot_mat);

    scene->Draw();

    // Šî’êŠÖ”•`‰æ
    //DrawBsplineFunc(4, 6, 10, a, -3.0, 6.0);
    //DrawBsplineFunc(4, 6, 10, b, 0.0, 3.0);
    //DrawBsplineFunc(4, 6, 10, c, 0.0, 3.0);

    // Šô‰½ƒOƒŠƒbƒh•`‰æ
    DrawGrid(GeoGrid2D());

    glPopMatrix();

    // Ž²•`‰æ
    glPushMatrix();
    glTranslated(-2.0, -2.0, -2.0);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    glutSwapBuffers();
}