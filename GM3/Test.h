#pragma once

void TestRegister();
void TestDraw();

void DrawAllKind(); // すべてのタイプの曲線/曲面を描画

// 基底関数およびその関数を使ってBスプライン曲線を描画する
// 中級編第1回/第2回課題
void DrawBsplineFunctions();
void DrawBsplineCurves();

// Nurbs曲線/曲面を用いて円や球を描画する
// 中級編第3回
void DrawCircle_CGS3();
void DrawSphere_CGS3();
void DrawCylinder_CGS3();

// 曲線を近似して描画する
// 中級編第4回
void DrawApproxCurve_CGS4();

// 曲面を近似して描画する
// 中級編第5回
void DrawApproxSurface_CGS5();

// 参照曲線から最近点を求めて描画する
// 中級編第6回
void DrawCurveNearest_CGS6();

// 参照曲線から最近点を求めて描画する
// 中級編第7回
void DrawSurfaceNearest_CGS7();

// 曲線/曲面の分割とボックス分割法による曲線と曲面の交点取得
// 中級編第8回
void DrawSplitCurve_CGS8();
void DrawSplitSurface_CGS8();
void DrawIntersectCurveSurface_CGS8();
void DrawIntersectCurveSurface_CGS8_one();

// UV曲線の生成
// 中級編第9回
void DrawUV_CGS9();
void DrawXYZ_CGS9();
void CalcArea_CGS9();
