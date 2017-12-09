#define GLOBAL_INSTANCE 
#include "GV.h"

#include "Callback.h"

int main(int argc, char* argv[])
{
    // gl初期化
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

    // ウィンドウ設定
    glutInitWindowSize(DEF_WIN_X, DEF_WIN_Y);
    glutInitWindowPosition(90, 20);
    glutCreateWindow("形状処理");

    // 各種コールバック
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMouseWheelFunc(Wheel);

    // 初期化
    Initialize();

    glutMainLoop();
}