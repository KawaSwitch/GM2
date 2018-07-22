#pragma once

#include "GV.h"

// �������ɸ�Υǥץ��ͤ��������
float GetDepth(int x, int y);

// �������ɸ -> ���ɺ�ɸ
Vector3d GetWorldCoord(int x, int y, float depth);
// ���ɺ�ɸ -> �������ɸ
Vector3d GetLocalCoord(int x, int y, int z);

unsigned int GetObjNumberOnMousePointer(int x, int y);

unsigned int GetNearestNumber(GLuint hits, GLuint *buf);
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf);
