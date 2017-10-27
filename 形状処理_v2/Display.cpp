#include <gl/freeglut.h>
#include "Callback.h"
#include "GeoGrid.h"

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    // Šô‰½ƒOƒŠƒbƒh•`‰æ
    glPushMatrix();
    DrawGrid(GeoGrid2D());
    glPopMatrix();

    glutSwapBuffers();
}