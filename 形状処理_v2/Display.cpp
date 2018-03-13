#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Background.h"
#include "Axis.h"
#include "ControlPoint.h"
#include "BsplineCurve.h"
#include "Test.h"
#include "Picking.h"

NormalAxis* axis; // 軸
Scene* scene; // シーン
extern Scene* test_scene;

extern Point3d center;

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // ---- ビュー行列 ----
    glLoadIdentity();
    gluLookAt(
        0.0, 0.0, 9.0,  // 視点位置
        0.0, 0.0, 0.0,  // 注視位置
        0.0, 1.0, 0.0   // 上方向 : y
    );
    // --------

    // ---- モデル行列 ----

    glEnable(GL_STENCIL_TEST); // ステンシル有効化

    // 1. 形状描画
    glPushMatrix();

    // 形状描画の型取り
    glStencilFunc(GL_ALWAYS, static_cast<int>(StencilRef::Entity), 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

    glTranslated(dist_X, -dist_Y, dist_Z); // 移動
    glMultMatrixd(rot_mat); // 回転

    // 幾何グリッド描画
    DrawGrid(GeoGrid2D());

    // 形状描画
    scene->Draw();
    // テスト描画
    TestDraw();

    glPopMatrix();


    // 2. 背景描画
    glPushMatrix();
    
    // 形状描画されていない箇所は背景を描画
    glStencilFunc(GL_NOTEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    DrawBackground(BackgroundNormal());

    glPopMatrix();

    glDisable(GL_STENCIL_TEST); // ステンシル無効化


    // 3.軸描画
    // TODO: ビューポート別
    glPushMatrix();
    glScaled(1.3, 1.3, 1.3);
    glTranslated(-1.6, -1.6, -1.6);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    // --------

    glutSwapBuffers();
}