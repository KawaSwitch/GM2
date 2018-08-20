#pragma once

#include "GV.h"
#include "Picking.h"
#include "Camera.h"
#include "CalcUtil.h"

static GLdouble color_top[4] = {1.0, 1.0, 1.0, 1.0};
static GLdouble color_bottom[4] = {0.8, 1.0, 0.9, 1.0};

template <class IBackGround>
void DrawBackground(const IBackGround &grid)
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
        double elev_angle = PersParam::fovy / 2; // 仰角(度数法)
        double backHeightHalf, backWidthHalf;    // 背景の大きさ(原点からの長さ)

        glPushMatrix();
        {
            // 視体積に合うように背景の大きさと位置を決定する
            // 現状メインビューは透視投影のみ
            if (mainProjType == ProjectType::Perspective)
            {
                // 視錐体前面の縦幅横幅を計算
                backHeightHalf = PersParam::zNear * std::tan(ToRad(elev_angle));
                backWidthHalf = backHeightHalf * ((GLdouble)width / height);

                // 視錐体の前面に設定する
                // NOTE: 後面だとなぜかずらす必要があったので前面にした
                glTranslated(0, 0, -PersParam::zNear);
            }
            else
                Error::ShowAndExit("背景設定不可", "投影法が無効な値です");

            // 背景描画
            glBegin(GL_QUADS);

            glColor4dv(color_top);
            glVertex3d(-backWidthHalf, backHeightHalf, 0);
            glVertex3d(backWidthHalf, backHeightHalf, 0);

            glColor4dv(color_bottom);
            glVertex3d(backWidthHalf, -backHeightHalf, 0);
            glVertex3d(-backWidthHalf, -backHeightHalf, 0);

            glEnd();
        }
        glPopMatrix();
    }
};
