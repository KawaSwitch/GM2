#pragma once

#include "GV.h"

// ローカル座標のデプス値を取得する
float GetDepth(int x, int y);

// ローカル座標 -> ワールド座標
Vector3d GetWorldCoord(int x, int y, float depth);
// ワールド座標 -> ローカル座標
Vector3d GetLocalCoord(int x, int y, int z);

unsigned int GetObjNumberOnMousePointer(int x, int y);

unsigned int GetNearestNumber(GLuint hits, GLuint *buf);
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf);
