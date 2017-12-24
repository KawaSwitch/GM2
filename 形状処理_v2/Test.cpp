#define _USE_MATH_DEFINES

#include "GV.h"
#include <math.h>
#include "Test.h"
#include "Model.h"
#include "Reader.h"
#include "Scene.h"

static int current_size; // 現在のテスト用のディスプレイリスト数
static int* displayLists; // ディスプレイリスト保管用ポインタ
static bool isFirst = true;

Scene* test_scene;

// 描画したいのをおく
static vector<function<void(void)>> TestRegisterDraw
{
    //DrawBsplineFunctions, // Bスプライン基底関数描画
    //DrawBsplineCurves, // Bスプライン曲線描画
    //TestGetNearestPointCurveToCurve_CGS04, // 曲線と曲線の最近点群描画
    //TestGetNearestPointCurveToSurface_CGS04, // 曲線と曲面の最近点群描画
    //ShowLUDecomp, // LU分解の結果を確かめる
    //DrawCircle_CGS3, // Nurbs曲線で円描く
    //DrawSphere_CGS3, // Nurbs曲面で球を描く
    DrawCylinder_CGS3, // Nurbs曲面で円柱を描く
};

// Nurbs曲面で円柱を描く
void DrawCylinder_CGS3()
{
    // よくないけど無理やり求めた
    double cp1_p[2] = { 5, 5 * sqrt(3) };
    RotateCoord2DAroundOrigin(cp1_p, (double)2 / 3 * M_PI);

    ControlPoint cp0[9]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 5 * sqrt(3), 0, (double)1 / 2),
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),

        ControlPoint(5, 0, 10, 1.0),
        ControlPoint(5, 5 * sqrt(3), 10, (double)1 / 2),
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
    };
    ControlPoint cp1[9]
    {
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),
        ControlPoint(cp1_p[0], cp1_p[1], 0, (double)1 / 2),
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),

        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
        ControlPoint(cp1_p[0], cp1_p[1], 10, (double)1 / 2),
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
    };
    ControlPoint cp2[9]
    {
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),
        ControlPoint(5, -5 * sqrt(3), 0, (double)1 / 2),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
        ControlPoint(5, -5 * sqrt(3), 10, (double)1 / 2),
        ControlPoint(5, 0, 10, 1.0),
    };


    double knotU[6] = { 0, 0, 0, 1, 1, 1 };
    double knotV[4] = { 0, 0, 1, 1 };

    NurbsSurface* surf0 = new NurbsSurface(3, 2, cp0, 3, 2, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf1 = new NurbsSurface(3, 2, cp1, 3, 2, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf2 = new NurbsSurface(3, 2, cp2, 3, 2, knotU, knotV, Color::green_alpha);

    if (isFirst)
    {
        test_scene->AddObject(surf0);
        test_scene->AddObject(surf1);
        test_scene->AddObject(surf2);
    }
}

// Nurbs曲面で球を描く
void DrawSphere_CGS3()
{
    ControlPoint cp0[9]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 0, -5, 1 / sqrt(2)),
        ControlPoint(0, 0, -5, 1.0),

        ControlPoint(5, 5, 0, 1 / sqrt(2)),
        ControlPoint(5, 5, -5, (double)1 / 2),
        ControlPoint(0, 5, -5, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp1[9]
    {
        ControlPoint(0, 0, 5, 1.0),
        ControlPoint(5, 0, 5, 1 / sqrt(2)),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(0, 5, 5, 1 / sqrt(2)),
        ControlPoint(5, 5, 5, (double)1 / 2),
        ControlPoint(5, 5, 0, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp2[9]
    {
        ControlPoint(0, 0, -5, 1.0),
        ControlPoint(-5, 0, -5, 1 / sqrt(2)),
        ControlPoint(-5, 0, 0, 1.0),

        ControlPoint(0, 5, -5, 1 / sqrt(2)),
        ControlPoint(-5, 5, -5, (double)1 / 2),
        ControlPoint(-5, 5, 0, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp3[9]
    {
        ControlPoint(-5, 0, 0, 1.0),
        ControlPoint(-5, 0, 5, 1 / sqrt(2)),
        ControlPoint(0, 0, 5, 1.0),

        ControlPoint(-5, 5, 0, 1 / sqrt(2)),
        ControlPoint(-5, 5, 5, (double)1 / 2),
        ControlPoint(0, 5, 5, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp4[9]
    {
        ControlPoint(0, 0, -5, 1.0),
        ControlPoint(5, 0, -5, 1 / sqrt(2)),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(0, -5, -5, 1 / sqrt(2)),
        ControlPoint(5, -5, -5, (double)1 / 2),
        ControlPoint(5, -5, 0, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp5[9]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 0, 5, 1 / sqrt(2)),
        ControlPoint(0, 0, 5, 1.0),

        ControlPoint(5, -5, 0, 1 / sqrt(2)),
        ControlPoint(5, -5, 5, (double)1 / 2),
        ControlPoint(0, -5, 5, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp6[9]
    {
        ControlPoint(-5, 0, 0, 1.0),
        ControlPoint(-5, 0, -5, 1 / sqrt(2)),
        ControlPoint(0, 0, -5, 1.0),

        ControlPoint(-5, -5, 0, 1 / sqrt(2)),
        ControlPoint(-5, -5, -5, (double)1 / 2),
        ControlPoint(0, -5, -5, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp7[9]
    {
        ControlPoint(0, 0, 5, 1.0),
        ControlPoint(-5, 0, 5, 1 / sqrt(2)),
        ControlPoint(-5, 0, 0, 1.0),

        ControlPoint(0, -5, 5, 1 / sqrt(2)),
        ControlPoint(-5, -5, 5, (double)1 / 2),
        ControlPoint(-5, -5, 0, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };

    double knotU[6] = { 0, 0, 0, 1, 1, 1 };
    double knotV[6] = { 0, 0, 0, 1, 1, 1 };

    NurbsSurface* surf0 = new NurbsSurface(3, 3, cp0, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf1 = new NurbsSurface(3, 3, cp1, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf2 = new NurbsSurface(3, 3, cp2, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf3 = new NurbsSurface(3, 3, cp3, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf4 = new NurbsSurface(3, 3, cp4, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf5 = new NurbsSurface(3, 3, cp5, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf6 = new NurbsSurface(3, 3, cp6, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf7 = new NurbsSurface(3, 3, cp7, 3, 3, knotU, knotV, Color::green_alpha);

    if (isFirst)
    {
        test_scene->AddObject(surf0);
        test_scene->AddObject(surf1);
        test_scene->AddObject(surf2);
        test_scene->AddObject(surf3);
        test_scene->AddObject(surf4);
        test_scene->AddObject(surf5);
        test_scene->AddObject(surf6);
        test_scene->AddObject(surf7);
    }
}

// Nurbs曲線で円を描く
void DrawCircle_CGS3()
{
    ControlPoint cp0[3]
    {
        ControlPoint(5, 0, 0, sqrt(2)),
        ControlPoint(5, 5, 0, 1),
        ControlPoint(0, 5, 0, sqrt(2)),
    };
    ControlPoint cp1[3]
    {
        ControlPoint(0, 5, 0, sqrt(2)),
        ControlPoint(-5, 5, 0, 1),
        ControlPoint(-5, 0, 0, sqrt(2)),
    };
    ControlPoint cp2[3]
    {
        ControlPoint(-5, 0, 0, sqrt(2)),
        ControlPoint(-5, -5, 0, 1),
        ControlPoint(0, -5, 0, sqrt(2)),
    };
    ControlPoint cp3[3]
    {
        ControlPoint(0, -5, 0, sqrt(2)),
        ControlPoint(5, -5, 0, 1),
        ControlPoint(5, 0, 0, sqrt(2)),
    };

    double knot[6] = { 0, 0, 0, 1, 1, 1 };

    NurbsCurve* curve0 = new NurbsCurve(3, cp0, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve1 = new NurbsCurve(3, cp1, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve2 = new NurbsCurve(3, cp2, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve3 = new NurbsCurve(3, cp3, 3, knot, Color::blue, 2.0);

    if (isFirst)
    {
        test_scene->AddObject(curve0);
        test_scene->AddObject(curve1);
        test_scene->AddObject(curve2);
        test_scene->AddObject(curve3);
    }
}

// LU分解の結果を表示する
void ShowLUDecomp()
{
    double A[2][2] = { { 2, 1 }, { 3, 1 } };
    double B[2] = { 5, 6 };

    auto x = LUDecomposition<2>(A, B);
    printf("%f %f\n", x[0], x[1]);
}

// 参照曲線上に離散点を生成し, それを外部点として最近点を求める
// レジメ第04回_05月A.docx
void TestGetNearestPointCurveToCurve_CGS04()
{
    auto reader = new KjsReader("KJS_FILE/");

    // 対象曲線/曲面
    Curve* curve1 = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_1.kjs");
    // 参照曲線
    Curve* curveC = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_C.kjs");

    // 参照点群を取得
    auto ref_pnts = curveC->GetPositionVectors(20);
    vector<Vector3d> nearest_pnts; // 最近点群

    // 最近点取得
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(curve1->GetNearestPointFromRef(ref_pnts[i]));

    // 描画
    glLineWidth(1.0);
    glPointSize(5.0);
    glBegin(GL_LINES);
    glColor3dv(Color::orange);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
    {
        glVertex3d(ref_pnts[i]);
        glVertex3d(nearest_pnts[i]);
    }
    glEnd();

    glBegin(GL_POINTS);
    glColor3dv(Color::red);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        glVertex3d(ref_pnts[i]);

    glColor3dv(Color::light_green);
    for (int i = 0; i < (int)nearest_pnts.size(); i++)
        glVertex3d(nearest_pnts[i]);
    glEnd();

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
    Surface* surf1 = (Surface *)reader->GetObjectFromFile("CGS_bspline_surface_1.kjs");
    // 参照曲線
    Curve* curveS = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_S.kjs");

    // 参照点群を取得
    auto ref_pnts = curveS->GetPositionVectors(20);
    vector<Vector3d> nearest_pnts; // 最近点群

    // 最近点取得
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(surf1->GetNearestPointFromRef(ref_pnts[i]));

    // 描画
    glLineWidth(1.0);
    glPointSize(5.0);
    glBegin(GL_LINES);
    glColor3dv(Color::orange);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
    {
        glVertex3d(ref_pnts[i]);
        glVertex3d(nearest_pnts[i]);
    }
    glEnd();

    glBegin(GL_POINTS);
    glColor3dv(Color::red);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        glVertex3d(ref_pnts[i]);

    glColor3dv(Color::light_green);
    for (int i = 0; i < (int)nearest_pnts.size(); i++)
        glVertex3d(nearest_pnts[i]);
    glEnd();

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
    //DrawBsplineFunc(4, 6, 10, knot_a, -3.0, 6.0);
    DrawBsplineFunc(4, 6, 10, knot_b, 0.0, 3.0);
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

    // テストシーン描画
    test_scene->Draw();
}