#include "GV.h"
#include "CalcUtil.h"

// 点群を描画する
void DrawPoints(vector<Vector3d> vec, GLdouble* color, GLdouble pSize)
{
    glColor4dv(color);
    glPointSize(pSize);

    glBegin(GL_POINTS);
    {
        for (auto it = vec.begin(); it != vec.end(); ++it)
            glVertex3d((*it));
    }
    glEnd();

    glPointSize(1.0); // 戻しておく
}

// 基底関数を描画する
void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t, double max_t)
{
    // ディスプレイリスト
    static int displayList = 0;

    // 誤差防止
    int MIN = (int)(min_t * 100);
    int MAX = (int)(max_t * 100);

    glLineWidth(1.5);

    if (displayList != 0)
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
    }
}

// ディスプレイリストと描画関数を用いて描画する
void DrawUsingDisplayList(int* const displayList, const function<void(void)> RegisterDraw)
{
    if (*displayList == 0)
    {
        if (!(*displayList = glGenLists(1)))
            return;

        // 登録
        glNewList(*displayList, GL_COMPILE);
        RegisterDraw();
        glEndList();

        glutPostRedisplay();
    }
    else
        glCallList(*displayList);
}