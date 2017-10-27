#include "GV.h"
#include "Callback.h"

void Resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLdouble)w / h, 0.1, 10000.0);

    glMatrixMode(GL_MODELVIEW);
}