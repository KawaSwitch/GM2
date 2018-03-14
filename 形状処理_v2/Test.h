#pragma once

void TestDraw();

// 参照曲線上に離散点を生成し, それを外部点として最近点を求める
// レジメ第04回_05月A.docx
void TestGetNearestPointCurveToCurve_CGS04();
void TestGetNearestPointCurveToSurface_CGS04();

// 基底関数およびその関数を使ってBスプライン曲線を描画する
// 中級編第1回課題
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