#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "BezierCurve.h"
#include "BezierSurface.h"
#include "BsplineCurve.h"
#include "BsplineSurface.h"
#include "Triangle.h"

extern NormalAxis* axis;
extern Scene* scene;

double a[10] = { -3, -2, -1, 0, 1, 2, 3, 4, 5, 6 };
double b[10] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 3 };
double c[10] = { 0, 0, 0, 0, 1, 1, 3, 3, 3, 3 };
double surf1_u[10] = { 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 3.0, 3.0, 3.0 };
double surf1_v[7] = { 0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0 };
double curve2[8] = { 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 3.0, 3.0 };

void InitScene()
{
    scene = new Scene();

    ControlPoint cp_curve2[5] =
    {
        ControlPoint(-5, 2, 0),
        ControlPoint(-3, 10, 0),
        ControlPoint(1, 2, 0),
        ControlPoint(2, 5, 0),
        ControlPoint(4, 5, 0),
    };

    ControlPoint cp_sample[6] =
    {
        ControlPoint(-3, 2, 0),
        ControlPoint(-5, 4, 0),
        ControlPoint(-1, 8, 0),
        ControlPoint(2, 7, 0),
        ControlPoint(4, 3, 0),
        ControlPoint(6, 1, 0),
    };

    ControlPoint cp_surf1[24] =
    {
        ControlPoint(0, 0, 0),
        ControlPoint(10, 0, 5),
        ControlPoint(20, 0, 0),
        ControlPoint(30, 0, -5),
        ControlPoint(40, 0, -5),
        ControlPoint(50, 0, 0),

        ControlPoint(0, 10, 5),
        ControlPoint(10, 10, 0),
        ControlPoint(20, 10, 0),
        ControlPoint(30, 10, -2),
        ControlPoint(40, 10, -1),
        ControlPoint(50, 10, -1),

        ControlPoint(0, 20, -5),
        ControlPoint(10, 20, -5),
        ControlPoint(20, 20, -5),
        ControlPoint(30, 20, 3),
        ControlPoint(40, 20, 5),
        ControlPoint(50, 20, 5),

        ControlPoint(0, 30, 0),
        ControlPoint(10, 30, 0),
        ControlPoint(20, 30, -5),
        ControlPoint(30, 30, 3),
        ControlPoint(40, 30, 7),
        ControlPoint(50, 30, 7),
    };

    ControlPoint cp_bezierC[4] =
    {
        ControlPoint(1, 2, 0),
        ControlPoint(2, 3, 1),
        ControlPoint(3, 1, 2),
        ControlPoint(5, 1, 2),
    };

    ControlPoint cp_bezierS[16] =
    {
        ControlPoint(0, 0, 0),
        ControlPoint(10, 0, 10),
        ControlPoint(20, 0, 10),
        ControlPoint(30, 0, 0),

        ControlPoint(0, 10, 10),
        ControlPoint(10, 10, 20),
        ControlPoint(20, 10, 20),
        ControlPoint(30, 10, 20),

        ControlPoint(0, 20, 10),
        ControlPoint(10, 20, 10),
        ControlPoint(20, 20, 20),
        ControlPoint(30, 20, 20),
        
        ControlPoint(0, 30, 10),
        ControlPoint(10, 30, 20),
        ControlPoint(20, 30, 10),
        ControlPoint(30, 30, 20),
    };

    // 各オブジェクト追加
    scene->AddObject(new BsplineCurve(4, cp_sample, 6, a, Color::blue, 1.0));
    //scene->AddObject(new BsplineCurve(4, cp_sample, 6, b, Color::orange, 1.0));
    //scene->AddObject(new BsplineCurve(4, cp_sample, 6, c, Color::green, 1.0));
    scene->AddObject(new BsplineSurface(4, 3, cp_surf1, 6, 4, surf1_u, surf1_v, Color::blue_alpha, 1.0));
    //scene->AddObject(new BezierCurve(4, cp_bezierC, 4, Color::blue, 1.0));
    //scene->AddObject(new BezierSurface(4, 4, cp_bezierS, 4, 4, Color::green_alpha, 1.0));
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

    glEnable(GL_BLEND); // ブレンド 有効化
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mainLight.InitLight(); // メインライト初期化
    isUseLight = false; // デフォルトはオフ

    //// カリング処理
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    axis = new NormalAxis(0.4); // 軸

    obj_number = 1; // 初期識別子は1

    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;

    InitQuaternion();
    InitScene();
}