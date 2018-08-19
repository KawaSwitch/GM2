#pragma once

#include "GV.h"
#include "ControlPoint.h"
#include <float.h>

// ミニマクスボックス
class Box
{
  private:
    double _xmin, _ymin, _zmin;
    double _xmax, _ymax, _zmax;

  public:
    double MinX() const { return _xmin; }
    double MinY() const { return _ymin; }
    double MinZ() const { return _zmin; }
    double MaxX() const { return _xmax; }
    double MaxY() const { return _ymax; }
    double MaxZ() const { return _zmax; }

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

    // 対角線の長さ
    double DiagLength()
    {
        Vector3d diag(
            _xmax - _xmin,
            _ymax - _ymin,
            _zmax - _zmin);

        return diag.Length();
    }

    // 他ボックスと干渉するか
    bool IsInterfere(const Box &other)
    {
        return _xmax > other._xmin && _ymax > other._ymin && _zmax > other._zmin && other._xmax > _xmin && other._ymax > _ymin && other._zmax > _zmin;
    }

    // ボックスが適正か
    bool IsValid()
    {
        return _xmax >= _xmin && _ymax >= _ymin && _zmax >= _zmin;
    }

    // 描画
    void Draw(const GLdouble *const color, const GLdouble width) const
    {
        glColor4dv(color);
        glLineWidth((GLfloat)width);

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

    Box()
    {
        _xmin = 0;
        _ymin = 0;
        _zmin = 0;
        _xmax = 0;
        _ymax = 0;
        _zmax = 0;
    }

    Box(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
    {
        _xmin = xmin;
        _ymin = ymin;
        _zmin = zmin;
        _xmax = xmax;
        _ymax = ymax;
        _zmax = zmax;
    }

    Box(Vector3d min, Vector3d max)
    {
        _xmin = min.X;
        _ymin = min.Y;
        _zmin = min.Z;
        _xmax = max.X;
        _ymax = max.Y;
        _zmax = max.Z;
    }

    // 制御点を囲むボックスを取得(バウンディングボックス)
    Box(vector<ControlPoint> cp)
    {
        double xmin, ymin, zmin;
        double xmax, ymax, zmax;
        xmin = ymin = zmin = DBL_MAX;
        xmax = ymax = zmax = -DBL_MAX;

        // 最小/最大を見つける
        for (unsigned int i = 0; i < cp.size(); i++)
        {
            if (cp[i].X < xmin)
                xmin = cp[i].X;
            if (cp[i].Y < ymin)
                ymin = cp[i].Y;
            if (cp[i].Z < zmin)
                zmin = cp[i].Z;
            if (cp[i].X > xmax)
                xmax = cp[i].X;
            if (cp[i].Y > ymax)
                ymax = cp[i].Y;
            if (cp[i].Z > zmax)
                zmax = cp[i].Z;
        }

        _xmin = xmin;
        _ymin = ymin;
        _zmin = zmin;
        _xmax = xmax;
        _ymax = ymax;
        _zmax = zmax;
    }

    // すべてのボックスを囲むボックスを取得
    Box(vector<Box> box)
    {
        double xmin, ymin, zmin;
        double xmax, ymax, zmax;
        xmin = ymin = zmin = DBL_MAX;
        xmax = ymax = zmax = -DBL_MAX;

        // 最小/最大を見つける
        for (unsigned int i = 0; i < box.size(); i++)
        {
            // 疑: C++って同クラスだったら引数のメンバにアクセスしてよかったっけ?
            if (box[i]._xmin < xmin)
                xmin = box[i]._xmin;
            if (box[i]._ymin < ymin)
                ymin = box[i]._ymin;
            if (box[i]._zmin < zmin)
                zmin = box[i]._zmin;
            if (box[i]._xmax > xmax)
                xmax = box[i]._xmax;
            if (box[i]._ymax > ymax)
                ymax = box[i]._ymax;
            if (box[i]._zmax > zmax)
                zmax = box[i]._zmax;
        }

        _xmin = xmin;
        _ymin = ymin;
        _zmin = zmin;
        _xmax = xmax;
        _ymax = ymax;
        _zmax = zmax;
    }
};

// 2次元 ボックス
struct Box2d
{
  public:
    double MinX, MinY;
    double MaxX, MaxY;

  public:
    Box2d()
    {
        MinX = 0;
        MinY = 0;
        MaxX = 0;
        MaxY = 0;
    }

    Box2d(double minX, double minY, double maxX, double maxY)
    {
        MinX = minX;
        MinY = minY;
        MaxX = maxX;
        MaxY = maxY;
    }

    Point<double> Center()
    {
        return Point<double>{
            (MinX + MaxX) / 2.0,
            (MinY + MaxY) / 2.0};
    }
};
