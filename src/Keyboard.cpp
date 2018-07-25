#include "GV.h"
#include "Callback.h"
#include "Scene.h"
#include "Axis.h"

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

    case 'a': // 軸表示変更
      switch (axisShowType)
	{
	case AxisShowType::None:
	  axisShowType = AxisShowType::Normal;
	  break;
	case AxisShowType::Normal:
	  axisShowType = AxisShowType::WithLabel;
	  break;
	case AxisShowType::WithLabel:
	  axisShowType = AxisShowType::None;
	  break;
	default:
	  Error::ShowMessage("Axis Show Setting error");
	  break;
	}
      break;
    case 'g': // グリッド表示変更
      switch (gridType)
	{
	case GridType::InVisible:
	  gridType = GridType::NonAxis;
	  break;
	case GridType::NonAxis:
	  gridType = GridType::WithAxis;
	  break;
	case GridType::WithAxis:
	  gridType = GridType::InVisible;
	  break;
	default:
	  Error::ShowMessage("Grid Show Setting error");
	  break;
	}
      break;
    case 'i': // 初期位置の表示に戻す
      isViewInitRequested = true;
      break;
      // 拡大
    case 'j':
      dist_Z += 2.0;
      break;
    case 'h':
      dist_Z += 1.0;
      break;
      //縮小
    case 'k':
      dist_Z -= 2.0;
      break;
    case 'l':
      dist_Z -= 1.0;
      break;
      //case 'l': // ライティングトグル
      //    isUseLight = !isUseLight;
      //    break;
    default:
      break;
    }

  glutPostRedisplay();
}
