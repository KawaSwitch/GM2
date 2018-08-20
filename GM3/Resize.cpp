#include "GV.h"
#include "Callback.h"
#include "Background.h"

void Resize(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    // プロジェクションモードへ以降
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLdouble)w / h, 0.1, 10000.0);

    // モデルビューモードへ戻す
    glMatrixMode(GL_MODELVIEW);
}
