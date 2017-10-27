#include <gl/freeglut.h>
#include "Callback.h"
#include "Initialize.h"

int main(int argc, char* argv[])
{
    // gl初期化
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // ウィンドウ設定
    glutInitWindowSize(960, 960);
    glutInitWindowPosition(90, 20);
    glutCreateWindow("形状処理");

    // 各種コールバック
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);

    // 初期化
    Initialize();

    glutMainLoop();
}