#pragma once

#include "define.h"

// クォータニオン
struct Quaternion
{
    double x;
    double y;
    double z;
    double w;

    // 長さ
    double Length() const { return sqrt(x*x + y*y + z*z + w*w); }

    // 単位化
    Quaternion Normalize() const { return *this / Length(); }

    // 定数倍
    constexpr Quaternion operator*(double value) const
    {
        return{ x * value, y * value, z * value, w * value };
    }
    constexpr Quaternion operator/(double value) const
    {
        return{ x / value, y / value, z / value, w / value };
    }
};

// 2次元点
template<class T>
struct Point
{
    T x;
    T y;
};

// 曲面のUVパラメータ
enum ParamUV
{
    U,
    V,
};

// 曲面の端(曲線)
enum SurfaceEdge
{
    U_min, // Uパラメータが最小のときのアイソ曲線
    U_max, // 以下同様
    V_min,
    V_max,
};

// 選択データ
struct SelectionData
{
    unsigned int minZ;
    unsigned int maxZ;
    unsigned int name;

    SelectionData() { minZ = 0; maxZ = 0; name = 0; }
    SelectionData(unsigned int m, unsigned int M, unsigned int n)
    {
        minZ = m;
        maxZ = M;
        name = n;
    }
};

// ステンシル参照値
enum class StencilRef
{
    Background = 0, // 背景は0(初期値)
    Grid,
    Entity, // 曲線・曲面用
    RotateCenter, // 回転中心
    Axis,
    Ignore, // 意味なし Keepして使うのを想定
};

// グリッドの表示法
enum GridType
{
    NonAxis,
    WithAxis,
    InVisible,
};