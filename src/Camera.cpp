#include "Camera.h"

// �������e�萔
const double PersParam::fovy = 30; // y�����̎���p
const double PersParam::zNear = 0.1;
const double PersParam::zFar = 10000; // �K��

// ���e�@
ProjectType mainProjType = ProjectType::Perspective;
ProjectType axisProjType = ProjectType::Ortho;