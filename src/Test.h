#pragma once

void TestRegister();
void TestDraw();

// ���֐�����т��̊֐����g����B�X�v���C���Ȑ���`�悷��
// �����ґ�1��/��2��ۑ�
void DrawBsplineFunctions();
void DrawBsplineCurves();

// Nurbs�Ȑ�/�Ȗʂ�p���ĉ~�⋅��`�悷��
// �����ґ�3��
void DrawCircle_CGS3();
void DrawSphere_CGS3();
void DrawCylinder_CGS3();

// �Ȑ����ߎ����ĕ`�悷��
// �����ґ�4��
void DrawApproxCurve_CGS4();

// �Ȗʂ��ߎ����ĕ`�悷��
// �����ґ�5��
void DrawApproxSurface_CGS5();

// �Q�ƋȐ�����ŋߓ_�����߂ĕ`�悷��
// �����ґ�6��
void DrawCurveNearest_CGS6();

// �Q�ƋȐ�����ŋߓ_�����߂ĕ`�悷��
// �����ґ�7��
void DrawSurfaceNearest_CGS7();

// �Ȑ�/�Ȗʂ̕����ƃ{�b�N�X�����@�ɂ��Ȑ��ƋȖʂ̌�_�擾
// �����ґ�8��
void DrawSplitCurve_CGS8();
void DrawSplitSurface_CGS8();
void DrawIntersectCurveSurface_CGS8();