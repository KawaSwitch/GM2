#pragma once

#include "GV.h"
#include "ControlPoint.h"

// ミニマクスボックス
class Box
{
private:

    double _xmin, _ymin, _zmin;
    double _xmax, _ymax, _zmax;

public:

    // ボックスの体積
    double Volume()
    {
        return ((_xmax - _xmin) * (_ymax - _ymin) * (_zmax - _zmin));
    }

    // 中心座標
    Vector3d Center()
    {
        return Vector3d(
            (_xmin + _xmax) * 0.5,
            (_ymin + _ymax) * 0.5,
            (_zmin + _zmax) * 0.5);
    }

    // 他ボックスと干渉するか
    bool IsInterfere(const Box& other)
    {
        return _xmax >= other._xmin
            && _ymax >= other._ymin
            && _zmax >= other._zmin
            && other._xmax >= _xmin
            && other._ymax >= _ymin
            && other._zmax >= _zmin;
    }

    // 描画
    void Draw(GLdouble* color, GLdouble width)
    {
        glColor4dv(color);
        glLineWidth(width);

        glBegin(GL_LINE_LOOP);
        glVertex3d(_xmin, _ymin, _zmin);
        glVertex3d(_xmax, _ymin, _zmin);
        glVertex3d(_xmax, _ymax, _zmin);
        glVertex3d(_xmin, _ymax, _zmin);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3d(_xmin, _ymin, _zmax);
        glVertex3d(_xmax, _ymin, _zmax);
        glVertex3d(_xmax, _ymax, _zmax);
        glVertex3d(_xmin, _ymax, _zmax);
        glEnd();

        glBegin(GL_LINES);

        glVertex3d(_xmin, _ymin, _zmin);
        glVertex3d(_xmin, _ymin, _zmax);

        glVertex3d(_xmax, _ymin, _zmin);
        glVertex3d(_xmax, _ymin, _zmax);

        glVertex3d(_xmax, _ymax, _zmin);
        glVertex3d(_xmax, _ymax, _zmax);

        glVertex3d(_xmin, _ymax, _zmin);
        glVertex3d(_xmin, _ymax, _zmax);

        glEnd();

        glLineWidth(1.0f);
    }

    Box(vector<ControlPoint> cp)
    {
        double xmin, ymin, zmin;
        double xmax, ymax, zmax;
        xmin = ymin = zmin = DBL_MAX;
        xmax = ymax = zmax = -DBL_MAX;

        // 最小/最大を見つける
        for (unsigned int i = 0; i < cp.size(); i++)
        {
            if (cp[i].X < xmin) xmin = cp[i].X;
            if (cp[i].Y < ymin) ymin = cp[i].Y;
            if (cp[i].Z < zmin) zmin = cp[i].Z;
            if (cp[i].X > xmax) xmax = cp[i].X;
            if (cp[i].Y > ymax) ymax = cp[i].Y;
            if (cp[i].Z > zmax) zmax = cp[i].Z;
        }

        _xmin = xmin; _ymin = ymin; _zmin = zmin;
        _xmax = xmax; _ymax = ymax;  _zmax = zmax;
    }
};