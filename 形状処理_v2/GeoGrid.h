#pragma once

class GeoGrid2D
{
private:
    int _len; // 原点からの長さ
    int _skip; // グリッド間の長さ

    // 内部描画
    void DrawInternal()
    {
        glLineWidth(2.0);

        // 軸だけ色を別にして描く
        glBegin(GL_LINES);
        {
            // x軸
            glColor3dv(Color::red);
            glVertex2d(-_len, 0);
            glVertex2d(_len, 0);

            // y軸
            glColor3dv(Color::blue);
            glVertex2d(0, -_len);
            glVertex2d(0, _len);
        }
        glEnd();


        // 色：グレー
        glColor4d(0.8, 0.8, 0.8, 1.0);

        for (int x = -_len; x <= _len; x += _skip)
        {
            for (int y = -_len; y <= _len; y += _skip)
            {
                glBegin(GL_LINES);
                {
                    // x-direction
                    if (y != 0)
                    {
                        glVertex2d(-_len, y);
                        glVertex2d(_len, y);
                    }

                    // y-direction
                    if (x != 0)
                    {
                        glVertex2d(x, -_len);
                        glVertex2d(x, _len);
                    }
                }
                glEnd();
            }
        }

        glLineWidth(1.0);
    }

public:
    
    GeoGrid2D(int len, int skip) { _len = len; _skip = skip; }
    
    // ディスプレイリスト描画
    void Draw()
    {
        // ディスプレイリスト
        static int displayList;
        static bool isRendered = false;

        // 光源はオフ
        if (glIsEnabled(GL_LIGHTING))
            glDisable(GL_LIGHTING);

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

            // actual drawing
            glEnable(GL_DEPTH_TEST);
            this->DrawInternal();

            glEndList();
            isRendered = true;
        }
    }
};
