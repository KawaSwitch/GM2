#pragma once

// �������e�萔
struct PersParam
{
    static const double fovy; // y�����̎���p
    static const double zNear;
    static const double zFar;
};

// ���e�@
enum ProjectType
{
    Perspective,    // �������e
    Ortho,          // ���s���e
};

extern ProjectType mainProjType; // ���C���r���[�̓��e�@
extern ProjectType axisProjType; // ���̓��e�@