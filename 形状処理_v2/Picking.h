#pragma once

#include "GV.h"

// ローカル座標のデプス値を取得する
float GetDepth(int x, int y);

// デプス値からワールド座標を取得する
Point3d GetWorldCoord(int x, int y, float depth);

unsigned int GetObjNumberOnMousePointer(int x, int y);

unsigned int GetNearestNumber(GLuint hits, GLuint *buf);
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf);