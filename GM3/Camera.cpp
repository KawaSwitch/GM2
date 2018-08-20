#include "Camera.h"

// 透視投影定数
const double PersParam::fovy = 30; // y方向の視野角
const double PersParam::zNear = 0.1;
const double PersParam::zFar = 10000; // 適当

// 投影法
ProjectType mainProjType = ProjectType::Perspective;
ProjectType axisProjType = ProjectType::Ortho;
