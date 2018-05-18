#pragma once

struct EPS
{
    // 距離トレランス
    static const double DIST;
    static const double DIST_SQRT;

    // 体積トレランス
    static const double VOLUME;

    // 相違トレランス
    static const double DIFF;

    // 最近点用トレランス
    static const double NEAREST;
    static const int COUNT_MAX;
};
