#include "GV.h"

// 基底関数を描画する
void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t, double max_t)
{
    // ディスプレイリスト
    static int displayList;
    static bool isRendered = false;

    // 誤差防止
    int MIN = (int)(min_t * 100);
    int MAX = (int)(max_t * 100);

    glLineWidth(1.5);

    if (isRendered)
    {
        // ディスプレイリスト作成済みならコール
        glCallList(displayList);
    }
    else
    {
        if (!(displayList = glGenLists(1)))
            Error::ShowAndExit("ディスプレイリスト作成失敗");

        glNewList(displayList, GL_COMPILE);

        // 基底関数は制御点数個ある
        for (int i = 0; i < ncpnt; i++)
        {
            glBegin(GL_LINE_STRIP);

            // 色はランダム
            GLdouble color[4];
            Color::GetRandomColor(color);
            glColor4dv(color);

            for (int T = MIN; T <= MAX; T++)
            {
                double t = (double)T / 100.0;

                double bf = CalcBsplineFunc(i, mord, t, knot);
                glVertex3d(t, bf, 0);
            }

            glEnd();
        }

        glEndList();

        isRendered = true;
    }
}