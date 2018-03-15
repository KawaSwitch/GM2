#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <math.h>
#include <functional>
#include "define.h"
#include "EPS.h"
#include "Error.h"
#include "Color.h"
#include "Vector.h"
#include "Point3d.h"
#include "Matrix.h"
#include "Matrix3d.h"
#include "Light.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern 
#endif

#include "function.h"
#include "template.h"

GLOBAL int width, height; // ウィンドウサイズ

GLOBAL int mouse_X, mouse_Y; // マウス座標（回転用）
GLOBAL double xStart, yStart; // マウス座標（移動用）
GLOBAL double dist_X, dist_Y, dist_Z; // 移動距離

GLOBAL bool rotate_flag, move_flag; // 回転, 移動フラグ

GLOBAL double rot_mat[16]; // 回転マトリクス

GLOBAL unsigned int obj_number; // オブジェクト識別子

GLOBAL bool isUseLight; // ライティング処理を行うか

GLOBAL Matrix3d scene_mat; // シーン用同時変換行列

GLOBAL bool isShowAxis; // 軸を描画するか

GLOBAL bool isViewInitRequested; // 画面表示を初期に戻すか

// 構造体
GLOBAL Quaternion target;
GLOBAL Quaternion current;

// メインライト
GLOBAL Light* mainLight;
