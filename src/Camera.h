#pragma once

// 透視投影定数
struct PersParam
{
    static const double fovy; // y方向の視野角
    static const double zNear;
    static const double zFar;
};

// 投影法
enum ProjectType
{
    Perspective,    // 透視投影
    Ortho,          // 平行投影
};

extern ProjectType mainProjType; // メインビューの投影法
extern ProjectType axisProjType; // 軸の投影法
