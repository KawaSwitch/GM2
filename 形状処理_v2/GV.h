#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <functional>
#include <memory>
#include <vector>
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

#include "template.h"

GLOBAL int width, height; // �E�B���h�E�T�C�Y

GLOBAL int mouse_X, mouse_Y; // �}�E�X���W�i��]�p�j
GLOBAL double xStart, yStart; // �}�E�X���W�i�ړ��p�j
GLOBAL double dist_X, dist_Y, dist_Z; // �ړ�����

GLOBAL bool rotate_flag, move_flag; // ��], �ړ��t���O

GLOBAL double rot_mat[16]; // ��]�}�g���N�X

GLOBAL unsigned int obj_number; // �I�u�W�F�N�g���ʎq

GLOBAL bool isUseLight; // ���C�e�B���O�������s����

GLOBAL GridType gridType; // �O���b�h�`��^�C�v

GLOBAL bool isViewInitRequested; // ��ʕ\���������ɖ߂���

// �\����
GLOBAL Quaternion target;
GLOBAL Quaternion current;

// ���C�g
GLOBAL vector<Light *> lights;
GLOBAL vector<Light *> axisLight, modelLight;
