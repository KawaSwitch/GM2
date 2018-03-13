#pragma once

// TODO: 試したかったけど意味ないので時間あったらもうちょっと考える
template<class GeoGrid> void DrawGrid(const GeoGrid& grid)
{
    // 幾何グリッド描画
    grid.Draw();
}

// 2Dグリッド
struct GeoGrid2D
{
    void Draw() const
    {
        glDisable(GL_DEPTH_TEST);

        // 色：グレー
        glColor4d(0.8, 0.8, 0.8, 1.0);

        for (double x = -200; x <= 200; x += 5)
        {
            for (double y = -200; y <= 200; y += 5)
            {
                glBegin(GL_LINES);

                // x-direction
                glVertex3d(-200, y, 0);
                glVertex3d(200, y, 0);

                // y-direction
                glVertex3d(x, -200, 0);
                glVertex3d(x, 200, 0);

                glEnd();
            }
        }

        glEnable(GL_DEPTH_TEST);
    }
};

// 3Dグリッド
struct GeoGrid3D
{
    void Draw() const
    {

    }
};