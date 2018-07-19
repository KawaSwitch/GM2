#pragma once

#include "GV.h"

// ���[�J�����W�̃f�v�X�l���擾����
float GetDepth(int x, int y);

// ���[�J�����W -> ���[���h���W
Vector3d GetWorldCoord(int x, int y, float depth);
// ���[���h���W -> ���[�J�����W
Vector3d GetLocalCoord(int x, int y, int z);

unsigned int GetObjNumberOnMousePointer(int x, int y);

unsigned int GetNearestNumber(GLuint hits, GLuint *buf);
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf);