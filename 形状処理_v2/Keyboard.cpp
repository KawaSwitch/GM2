#include "GV.h"
#include "Callback.h"
#include "Scene.h"

extern Scene* scene;

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\033': // 終了
            exit(0);
            break;
        case 'p': // 制御点トグル
            scene->ToggleDrawControlPoints();
            break;
        case 'f': // 接線トグル
            scene->ToggleDrawFirstDiffVectors();
            break;
        case 's': // 2階微分ベクトルトグル
            scene->ToggleDrawSecondDiffVectors();
            break;
        case 'n': // 法線トグル
            scene->ToggleDrawNormalVectors();
            break;
        case 'c': // 曲率ベクトルトグル
            scene->ToggleDrawCurvatureVectors();
            break;
        case 'b': // ミニマクスボックストグル
            scene->ToggleDrawBox();
            break;
        //case 'l': // ライティングトグル
        //    isUseLight = !isUseLight;
        //    break;
        default:
            break;
    }

    glutPostRedisplay();
}