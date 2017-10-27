#pragma once

#include <gl/freeglut.h>
#include "define.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern 
#endif

#include "function.h"

GLOBAL int width, height; // ウィンドウサイズ