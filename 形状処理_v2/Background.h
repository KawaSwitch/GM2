#pragma once
#include "Picking.h"

static GLdouble color_top[4] = { 0.9, 0.9, 1.0, 1.0 };
static GLdouble color_middle[4] = { 1.0, 1.0, 1.0, 1.0 };
static GLdouble color_bottom[4] = { 0.0, 0.0, 0.1, 1.0 };

template<class IBackGround> void DrawBackground(const IBackGround& grid)
{
    grid.Draw();
}

// 背景インターフェース
class IBackGround
{
    void virtual Draw() const = 0;
};

// 通常背景
struct BackgroundNormal : IBackGround
{
    void Draw() const override
    {
        // デプス値は無効
        glDisable(GL_DEPTH_TEST);

        glBegin(GL_QUADS);

        // TODO: 視野きちんと計算したい
        glColor3dv(Color::red);
        glVertex3d(-5, 5, 0);

        glColor3dv(Color::green);
        glVertex3d(5, 5, 0);

        glColor3dv(Color::blue);
        glVertex3d(5, -5, 0);

        glColor3dv(Color::orange);
        glVertex3d(-5, -5, 0);
        
        //glColor3d(0.0, 0.8, 0.8);
        //glutSolidCube(1e4);
        //glutPostRedisplay();

        glEnd();

        glEnable(GL_DEPTH_TEST);
    }
};