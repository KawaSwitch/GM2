#define GLOBAL_INSTANCE 
#include "GV.h"

#include "Callback.h"
#include "Initialize.h"

int main(int argc, char* argv[])
{
    // gl������
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

    // �E�B���h�E�ݒ�
    glutInitWindowSize(DEF_WIN_X, DEF_WIN_Y);
    glutInitWindowPosition(90, 20);
    glutCreateWindow("Geometric Modeling g++ fork ver.");

    // �e��R�[���o�b�N
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMouseWheelFunc(Wheel);

    // ������
    Initialize();

    glutMainLoop();
}
