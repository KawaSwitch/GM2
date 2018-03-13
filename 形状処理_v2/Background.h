#pragma once
#include "Picking.h"

static GLdouble color_topLeft[4] = { 1.0, 0.9, 0.9, 1.0 };
static GLdouble color_topRight[4] = { 0.9, 1.0, 0.9, 1.0 };
static GLdouble color_btmRight[4] = { 0.9, 0.9, 1.0, 1.0 };
static GLdouble color_btmLeft[4] = { 1.0, 0.9, 0.7, 1.0 };
static GLdouble color_back[4] = { 0.9, 0.9, 1.0, 1.0 };

template<class IBackGround> void DrawBackground(const IBackGround& grid)
{
    grid.Draw();
}

// 背景インターフェース
class IBackGround
{
    void virtual Draw() const = 0;
};

static bool isBackNotDrawn = true;

// 通常背景
struct BackgroundNormal : IBackGround
{
    void Draw() const override
    {
        // デプス値は無効
        //glDisable(GL_DEPTH_TEST);

        const double R = 2.5; // 描画半径
        double aspect = (double)width / height;
        double aspectForWidth = aspect;
        double aspectForHeight = (aspect > 1.0) ? aspect : (double)1 / aspect;

        glBegin(GL_QUADS);

        // TODO: きちんと求めたい
        // 透視投影のモデル行列初期値(単位行列)のときのz成分に合わせただけ
        glColor3dv(color_topLeft);
        glVertex3d(-R * aspectForWidth, R * aspectForHeight, 0);

        glColor3dv(color_topRight);
        glVertex3d(R * aspectForWidth, R * aspectForHeight, 0);

        glColor3dv(color_btmRight);
        glVertex3d(R * aspectForWidth, -R * aspectForHeight, 0);

        glColor3dv(color_btmLeft);
        glVertex3d(-R * aspectForWidth, -R * aspectForHeight, 0);

        glEnd();

       //glEnable(GL_DEPTH_TEST);
    }
};