#define _USE_MATH_DEFINES

// compatibility with original GLUT
#define EXT_GLUT_WHEEL_UP 3
#define EXT_GLUT_WHEEL_DOWN 4

#include "GV.h"
#include "Scene.h"
#include "Callback.h"
#include "Quaternion.h"
#include "Picking.h"

extern Scene* scene;

void Mouse(int button, int state, int x, int y)
{ 
    // 右：回転
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
#ifdef _DEBUG
        //// ピクセルのデプス値を取得
        //float z = GetDepth(x, y);

        //// 押下場所の座標値を取得
        //auto coord = GetWorldCoord(x, y, z);

        //printf("mouse : x, y, depth : %d %d %f\n", x, y, z);
        //printf("world : x, y, z : %f %f %f\n\n", coord.X, coord.Y, coord.Z);
#endif

        // マウスボタンを押した位置を記憶
        mouse_X = x;
        mouse_Y = y;
        rotate_flag = GL_TRUE;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        rotate_flag = GL_FALSE;

        // 移動しているときだけ回転保存
        if (!(x == mouse_X && y == mouse_Y))
        {
            // 姿勢を保存
            current = target;
        }
    }

    // 左：移動
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        xStart = x;
        yStart = y;
        move_flag = GL_TRUE;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        move_flag = GL_FALSE;
    }

    // ホイール：拡大/縮小
    else if (button == EXT_GLUT_WHEEL_UP)
        dist_Z += 2.0;
    else if (button == EXT_GLUT_WHEEL_DOWN)
        dist_Z -= 2.0;

    // それ以外はどちらでもない
    else
    {
        rotate_flag = GL_FALSE;
        move_flag = GL_FALSE;
    }

    glutPostRedisplay(); // 途中でreturnで抜けるとここまで来ずに再描画できないので注意!
}

void Motion(int x, int y)
{
    if (rotate_flag == GL_FALSE && move_flag == GL_FALSE)
        return;

    if (rotate_flag)
    {
        // 移動量を計算
        double dx = (x - mouse_X) * 1.33 / width;
        double dy = (y - mouse_Y) * 1.0 / height;

        // クォータニオンの長さ
        double length = sqrt(dx * dx + dy * dy);

        if (length != 0.0)
        {
            double radian = length * M_PI;
            double theta = sin(radian) / length;

            // 回転後の姿勢
            Quaternion after = { dy * theta, dx * theta, 0.0, cos(radian) };

            target = after * current;
            CalcRotateMatrix(rot_mat, target);
        }
    }

    if (move_flag)
    {
        double xdis, ydis;
        double a = 0.5; // 調整用

        xdis = x - xStart;
        ydis = y - yStart;

        if (move_flag == GL_TRUE)
        {
            dist_X += xdis * a * a * a * a * a;
            dist_Y -= ydis * a * a * a * a * a; // ウィンドウ座標系に合わせる
        }

        xStart = x;
        yStart = y;
    }

    glutPostRedisplay();
}

void Wheel(int wheel_num, int direction, int x, int y)
{
    // ズーム
    if (direction == 1)
        dist_Z += 2.0;
    else if (direction == -1)
        dist_Z -= 2.0;

    glutPostRedisplay();
}
