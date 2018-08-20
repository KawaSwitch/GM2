#define GLOBAL_INSTANCE
#include "GV.h"

#include "Callback.h"
#include "Initialize.h"
#include <cstring>

int main(int argc, char *argv[])
{
	// ウィンドウタイトル
	char winTitle[256] = "Geometric Modeling";

	// gl初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	// ウィンドウ設定
	glutInitWindowSize(DEF_WIN_X, DEF_WIN_Y);
	glutInitWindowPosition(90, 20);

	// タイトル設定
	if (argc == 1)
		glutCreateWindow(winTitle);
	else if (argc == 2)
	{
		strcat_s(winTitle, " ");
		strcat_s(winTitle, argv[1]);
		glutCreateWindow(winTitle);
	}
	else
		perror("main");

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
