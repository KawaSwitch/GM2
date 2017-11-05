#include "Test.h"
#include "Model.h"
#include "Reader.h"
#include "Scene.h"

#define DISPLAY_LIST_MAX 128

static int current_size; // 現在のテスト用のディスプレイリスト数
static int* displayLists; // ディスプレイリスト保管用ポインタ
static bool isFirst = true;

Scene* test_scene;

// 描画したいのをおく
static vector<function<void(void)>> TestRegisterDraw
{
    TestGetNearestPointCurveToCurve_CGS04,
    TestGetNearestPointCurveToSurface_CGS04,
};

// 参照曲線上に離散点を生成し, それを外部点として最近点を求める
// レジメ第04回_05月A.docx
void TestGetNearestPointCurveToCurve_CGS04()
{
    auto reader = new KjsReader("KJS_FILE/");

    // 対象曲線/曲面
    auto curve1 = reader->GetObjectFromFile("CGS_bspline_curve_1.kjs");
    // 参照曲線
    auto curveC = reader->GetObjectFromFile("CGS_bspline_curve_C.kjs");

    if (isFirst)
    {
        test_scene->AddObject(curve1);
        test_scene->AddObject(curveC);
    }
}
void TestGetNearestPointCurveToSurface_CGS04()
{
    auto reader = new KjsReader("KJS_FILE/");

    // 対象曲線/曲面
    auto surf1 = reader->GetObjectFromFile("CGS_bspline_surface_1.kjs");
    // 参照曲線
    auto curveS = reader->GetObjectFromFile("CGS_bspline_curve_S.kjs");

    if (isFirst)
    {
        test_scene->AddObject(surf1);
        test_scene->AddObject(curveS);
    }
}

// 基底関数およびその関数を使ってBスプライン曲線を描画する
// 第1回課題
static double knot_a[10] = { -3, -2, -1, 0, 1, 2, 3, 4, 5, 6 };
static double knot_b[10] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 3 };
static double knot_c[10] = { 0, 0, 0, 0, 1, 1, 3, 3, 3, 3 };
void DrawBsplineFunctions()
{
    // 別のとこでディスプレイリストをstaticで使ってるので同時には書けない
    DrawBsplineFunc(4, 6, 10, knot_a, -3.0, 6.0);
    //DrawBsplineFunc(4, 6, 10, knot_b, 0.0, 3.0);
    //DrawBsplineFunc(4, 6, 10, knot_c, 0.0, 3.0);
}
void DrawBsplineCurves()
{
    static ControlPoint cp[6] =
    {
        ControlPoint(30, 80, 0),
        ControlPoint(50, 110, 0),
        ControlPoint(70, 120, 0),
        ControlPoint(90, 70, 0),
        ControlPoint(110, 60, 0),
        ControlPoint(130, 80, 0),
    };

    auto A = new BsplineCurve(4, cp, 6, knot_a, Color::blue, 1.0);
    auto B = new BsplineCurve(4, cp, 6, knot_b, Color::orange, 1.0);
    auto C = new BsplineCurve(4, cp, 6, knot_c, Color::green, 1.0);

    // 制御点込みで描画
    A->DrawAsItIsWithCPs();
    B->DrawAsItIsWithCPs();
    C->DrawAsItIsWithCPs();
}

// テスト描画メイン
void TestDraw()
{
    // 初回はディスプレイリスト作成
    if (isFirst)
    {
        for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
        {
            displayLists = (int *)realloc(displayLists, sizeof(int) * ++current_size);
            displayLists[i] = glGenLists(1);

            glNewList(displayLists[i], GL_COMPILE);
            TestRegisterDraw[i]();
            glEndList();
        }

        isFirst = false;
        glutPostRedisplay();
    }
    // それ以外はコール
    else
    {
        for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
        {
            if (displayLists[i])
                glCallList(displayLists[i]);
        }
    }
}