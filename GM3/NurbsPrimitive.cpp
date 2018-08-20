#define _USE_MATH_DEFINES

#include "GV.h"
#include "Scene.h"
#include "NurbsCurve.h"
#include "NurbsSurface.h"

extern Scene *scene;

//// Nurbs曲面で円柱を描く
//void DrawCylinder_CGS3()
//{
//    static bool isFirst = true;
//
//    // よくないけど無理やり求めた
//    double cp1_p[2] = { 5, 5 * sqrt(3) };
//    RotateCoord2DAroundOrigin(cp1_p, (double)2 / 3 * M_PI);
//
//    ControlPoint cp0[9]
//    {
//        ControlPoint(5, 0, 0, 1.0),
//        ControlPoint(5, 5 * sqrt(3), 0, (double)1 / 2),
//        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),
//
//        ControlPoint(5, 0, 10, 1.0),
//        ControlPoint(5, 5 * sqrt(3), 10, (double)1 / 2),
//        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
//    };
//    ControlPoint cp1[9]
//    {
//        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),
//        ControlPoint(cp1_p[0], cp1_p[1], 0, (double)1 / 2),
//        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),
//
//        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
//        ControlPoint(cp1_p[0], cp1_p[1], 10, (double)1 / 2),
//        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
//    };
//    ControlPoint cp2[9]
//    {
//        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),
//        ControlPoint(5, -5 * sqrt(3), 0, (double)1 / 2),
//        ControlPoint(5, 0, 0, 1.0),
//
//        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
//        ControlPoint(5, -5 * sqrt(3), 10, (double)1 / 2),
//        ControlPoint(5, 0, 10, 1.0),
//    };
//
//
//    double knotU[6] = { 0, 0, 0, 1, 1, 1 };
//    double knotV[4] = { 0, 0, 1, 1 };
//
//    NurbsSurface* surf0 = new NurbsSurface(3, 2, cp0, 3, 2, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf1 = new NurbsSurface(3, 2, cp1, 3, 2, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf2 = new NurbsSurface(3, 2, cp2, 3, 2, knotU, knotV, Color::green_alpha);
//
//    if (isFirst)
//    {
//        scene->AddObject(surf0);
//        scene->AddObject(surf1);
//        scene->AddObject(surf2);
//
//        isFirst = false;
//    }
//}
//
//// Nurbs曲面で球を描く
//void DrawSphere_CGS3()
//{
//    ControlPoint cp0[9]
//    {
//        ControlPoint(5, 0, 0, 1.0),
//        ControlPoint(5, 0, -5, 1 / sqrt(2)),
//        ControlPoint(0, 0, -5, 1.0),
//
//        ControlPoint(5, 5, 0, 1 / sqrt(2)),
//        ControlPoint(5, 5, -5, (double)1 / 2),
//        ControlPoint(0, 5, -5, 1 / sqrt(2)),
//
//        ControlPoint(0, 5, 0, 1.0),
//        ControlPoint(0, 5, 0, 1 / sqrt(2)),
//        ControlPoint(0, 5, 0, 1.0),
//    };
//    ControlPoint cp1[9]
//    {
//        ControlPoint(0, 0, 5, 1.0),
//        ControlPoint(5, 0, 5, 1 / sqrt(2)),
//        ControlPoint(5, 0, 0, 1.0),
//
//        ControlPoint(0, 5, 5, 1 / sqrt(2)),
//        ControlPoint(5, 5, 5, (double)1 / 2),
//        ControlPoint(5, 5, 0, 1 / sqrt(2)),
//
//        ControlPoint(0, 5, 0, 1.0),
//        ControlPoint(0, 5, 0, 1 / sqrt(2)),
//        ControlPoint(0, 5, 0, 1.0),
//    };
//    ControlPoint cp2[9]
//    {
//        ControlPoint(0, 0, -5, 1.0),
//        ControlPoint(-5, 0, -5, 1 / sqrt(2)),
//        ControlPoint(-5, 0, 0, 1.0),
//
//        ControlPoint(0, 5, -5, 1 / sqrt(2)),
//        ControlPoint(-5, 5, -5, (double)1 / 2),
//        ControlPoint(-5, 5, 0, 1 / sqrt(2)),
//
//        ControlPoint(0, 5, 0, 1.0),
//        ControlPoint(0, 5, 0, 1 / sqrt(2)),
//        ControlPoint(0, 5, 0, 1.0),
//    };
//    ControlPoint cp3[9]
//    {
//        ControlPoint(-5, 0, 0, 1.0),
//        ControlPoint(-5, 0, 5, 1 / sqrt(2)),
//        ControlPoint(0, 0, 5, 1.0),
//
//        ControlPoint(-5, 5, 0, 1 / sqrt(2)),
//        ControlPoint(-5, 5, 5, (double)1 / 2),
//        ControlPoint(0, 5, 5, 1 / sqrt(2)),
//
//        ControlPoint(0, 5, 0, 1.0),
//        ControlPoint(0, 5, 0, 1 / sqrt(2)),
//        ControlPoint(0, 5, 0, 1.0),
//    };
//    ControlPoint cp4[9]
//    {
//        ControlPoint(0, 0, -5, 1.0),
//        ControlPoint(5, 0, -5, 1 / sqrt(2)),
//        ControlPoint(5, 0, 0, 1.0),
//
//        ControlPoint(0, -5, -5, 1 / sqrt(2)),
//        ControlPoint(5, -5, -5, (double)1 / 2),
//        ControlPoint(5, -5, 0, 1 / sqrt(2)),
//
//        ControlPoint(0, -5, 0, 1.0),
//        ControlPoint(0, -5, 0, 1 / sqrt(2)),
//        ControlPoint(0, -5, 0, 1.0),
//    };
//    ControlPoint cp5[9]
//    {
//        ControlPoint(5, 0, 0, 1.0),
//        ControlPoint(5, 0, 5, 1 / sqrt(2)),
//        ControlPoint(0, 0, 5, 1.0),
//
//        ControlPoint(5, -5, 0, 1 / sqrt(2)),
//        ControlPoint(5, -5, 5, (double)1 / 2),
//        ControlPoint(0, -5, 5, 1 / sqrt(2)),
//
//        ControlPoint(0, -5, 0, 1.0),
//        ControlPoint(0, -5, 0, 1 / sqrt(2)),
//        ControlPoint(0, -5, 0, 1.0),
//    };
//    ControlPoint cp6[9]
//    {
//        ControlPoint(-5, 0, 0, 1.0),
//        ControlPoint(-5, 0, -5, 1 / sqrt(2)),
//        ControlPoint(0, 0, -5, 1.0),
//
//        ControlPoint(-5, -5, 0, 1 / sqrt(2)),
//        ControlPoint(-5, -5, -5, (double)1 / 2),
//        ControlPoint(0, -5, -5, 1 / sqrt(2)),
//
//        ControlPoint(0, -5, 0, 1.0),
//        ControlPoint(0, -5, 0, 1 / sqrt(2)),
//        ControlPoint(0, -5, 0, 1.0),
//    };
//    ControlPoint cp7[9]
//    {
//        ControlPoint(0, 0, 5, 1.0),
//        ControlPoint(-5, 0, 5, 1 / sqrt(2)),
//        ControlPoint(-5, 0, 0, 1.0),
//
//        ControlPoint(0, -5, 5, 1 / sqrt(2)),
//        ControlPoint(-5, -5, 5, (double)1 / 2),
//        ControlPoint(-5, -5, 0, 1 / sqrt(2)),
//
//        ControlPoint(0, -5, 0, 1.0),
//        ControlPoint(0, -5, 0, 1 / sqrt(2)),
//        ControlPoint(0, -5, 0, 1.0),
//    };
//
//    double knotU[6] = { 0, 0, 0, 1, 1, 1 };
//    double knotV[6] = { 0, 0, 0, 1, 1, 1 };
//
//    NurbsSurface* surf0 = new NurbsSurface(3, 3, cp0, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf1 = new NurbsSurface(3, 3, cp1, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf2 = new NurbsSurface(3, 3, cp2, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf3 = new NurbsSurface(3, 3, cp3, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf4 = new NurbsSurface(3, 3, cp4, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf5 = new NurbsSurface(3, 3, cp5, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf6 = new NurbsSurface(3, 3, cp6, 3, 3, knotU, knotV, Color::green_alpha);
//    NurbsSurface* surf7 = new NurbsSurface(3, 3, cp7, 3, 3, knotU, knotV, Color::green_alpha);
//
//    if (isFirst)
//    {
//        test_scene->AddObject(surf0);
//        test_scene->AddObject(surf1);
//        test_scene->AddObject(surf2);
//        test_scene->AddObject(surf3);
//        test_scene->AddObject(surf4);
//        test_scene->AddObject(surf5);
//        test_scene->AddObject(surf6);
//        test_scene->AddObject(surf7);
//    }
//}
//
//// Nurbs曲線で円を描く
//void DrawCircle_CGS3()
//{
//    ControlPoint cp0[3]
//    {
//        ControlPoint(5, 0, 0, sqrt(2)),
//        ControlPoint(5, 5, 0, 1),
//        ControlPoint(0, 5, 0, sqrt(2)),
//    };
//    ControlPoint cp1[3]
//    {
//        ControlPoint(0, 5, 0, sqrt(2)),
//        ControlPoint(-5, 5, 0, 1),
//        ControlPoint(-5, 0, 0, sqrt(2)),
//    };
//    ControlPoint cp2[3]
//    {
//        ControlPoint(-5, 0, 0, sqrt(2)),
//        ControlPoint(-5, -5, 0, 1),
//        ControlPoint(0, -5, 0, sqrt(2)),
//    };
//    ControlPoint cp3[3]
//    {
//        ControlPoint(0, -5, 0, sqrt(2)),
//        ControlPoint(5, -5, 0, 1),
//        ControlPoint(5, 0, 0, sqrt(2)),
//    };
//
//    double knot[6] = { 0, 0, 0, 1, 1, 1 };
//
//    NurbsCurve* curve0 = new NurbsCurve(3, cp0, 3, knot, Color::blue, 2.0);
//    NurbsCurve* curve1 = new NurbsCurve(3, cp1, 3, knot, Color::blue, 2.0);
//    NurbsCurve* curve2 = new NurbsCurve(3, cp2, 3, knot, Color::blue, 2.0);
//    NurbsCurve* curve3 = new NurbsCurve(3, cp3, 3, knot, Color::blue, 2.0);
//
//    if (isFirst)
//    {
//        test_scene->AddObject(curve0);
//        test_scene->AddObject(curve1);
//        test_scene->AddObject(curve2);
//        test_scene->AddObject(curve3);
//    }
//}
