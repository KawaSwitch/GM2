#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"

extern NormalAxis* axis;
extern Scene* scene;

void InitScene()
{
    scene = new Scene();

    // 各オブジェクト追加

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
    glClearColor(1.0, 1.0, 1.0, 1.0);   // 背景色：白

    glEnable(GL_DEPTH_TEST); // デプス値 有効化

    axis = new NormalAxis(0.4); // 軸

    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;

    InitQuaternion();
    InitScene();
}