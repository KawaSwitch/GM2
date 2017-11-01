#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "BsplineCurve.h"
#include "Triangle.h"

extern NormalAxis* axis;
extern Scene* scene;

double a[10] = { -3, -2, -1, 0, 1, 2, 3, 4, 5, 6 };
double b[10] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 3 };
double c[10] = { 0, 0, 0, 0, 1, 1, 3, 3, 3, 3 };

void InitScene()
{
    scene = new Scene();

    ControlPoint cp_sample[6] =
    {
        ControlPoint(-3, 2, 0),
        ControlPoint(-5, 4, 0),
        ControlPoint(-1, 8, 0),
        ControlPoint(2, 7, 0),
        ControlPoint(4, 3, 0),
        ControlPoint(6, 1, 0),
    };

    // 各オブジェクト追加
    scene->AddObject(new BsplineCurve(4, cp_sample, 6, a, Color::blue, 2.0));
    scene->AddObject(new BsplineCurve(4, cp_sample, 6, b, Color::orange, 2.0));
    scene->AddObject(new BsplineCurve(4, cp_sample, 6, c, Color::green, 2.0));
}

void InitQuaternion()
{
    // 回転行列
    for (int i = 0; i < 16; i++)
        rot_mat[i] = 0.0;

    // クォータニオン
    target.x = 0.0; target.y = 0.0;
    target.z = 0.0; target.w = 0.0;
    current.x = 0.0; current.y = 0.0;
    current.z = 0.0; current.w = 1.0;

    // クォータニオン初期化
    CalcRotateMatrix(rot_mat, current);
}

void Initialize()
{
    glewInit(); // glew拡張

    glClearColor(1.0, 1.0, 1.0, 1.0);   // 背景色：白

    glEnable(GL_DEPTH_TEST); // デプス値 有効化

    axis = new NormalAxis(0.4); // 軸

    obj_number = 1; // 初期識別子は1

    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;

    InitQuaternion();
    InitScene();
}