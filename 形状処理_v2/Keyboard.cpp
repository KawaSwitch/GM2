#include "GV.h"
#include "Callback.h"
#include "Scene.h"

extern Scene* scene;
extern Scene* test_scene;

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\033': // 終了
            exit(0);
            break;
        case 'p': // 制御点トグル
            scene->ToggleDrawControlPoints();
            test_scene->ToggleDrawControlPoints();
            break;
        case 'f': // 接線トグル
            scene->ToggleDrawFirstDiffVectors();
            test_scene->ToggleDrawFirstDiffVectors();
            break;
        case 's': // 2階微分ベクトルトグル
            scene->ToggleDrawSecondDiffVectors();
            test_scene->ToggleDrawSecondDiffVectors();
            break;
        case 'n': // 法線トグル
            scene->ToggleDrawNormalVectors();
            test_scene->ToggleDrawNormalVectors();
            break;
        case 'c': // 曲率ベクトルトグル
            scene->ToggleDrawCurvatureVectors();
            test_scene->ToggleDrawCurvatureVectors();
            break;
        case 'b': // ミニマクスボックストグル
            scene->ToggleDrawBox();
            test_scene->ToggleDrawBox();
            break;
        //case 'l': // ライティングトグル
        //    isUseLight = !isUseLight;
        //    break;
        default:
            break;
    }

    glutPostRedisplay();
}