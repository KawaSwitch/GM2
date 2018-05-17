#pragma once

#include "Vector.h"

// 最近点情報まとめ構造体
struct NearestPointInfo
{
    Vector3d nearestPnt; // 最近点
    double dist; // 最近点から参照点までの距離

    NearestPointInfo(Vector3d nearest, Vector3d ref)
    {
        nearestPnt = nearest;
        dist = nearest.DistanceFrom(ref);
    }
};

// 最近点情報まとめ構造体 曲線用
struct NearestPointInfoC : public NearestPointInfo
{
public:

    double param; // パラメータ位置

    NearestPointInfoC(Vector3d nearest, Vector3d ref, double t)
        : NearestPointInfo(nearest, ref)
    {
        param = t;
    }
};

// 最近点情報まとめ構造体 曲面用
struct NearestPointInfoS : public NearestPointInfo
{
public:

    double paramU, paramV; // パラメータ位置

    NearestPointInfoS(Vector3d nearest, Vector3d ref, double u, double v)
        : NearestPointInfo(nearest, ref)
    {
        paramU = u;
        paramV = v;
    }
};