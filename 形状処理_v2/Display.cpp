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

    // 形状が描画されていない箇所は背景を描画
    glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::Background), 0xFF);
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

// コンソールに説明を表示します
void ConsoleDiscription()
{
    puts("形状処理C++ 操作方法\n");

    puts("ESC : 終了\n");

    puts("$ ファイル起動時読み込み");
    puts("./KJS_FILE内で名前の先頭に@をつけた.kjsファイルを起動時に読み込みます\n");

    // エンティティ操作ボタン説明
    puts("$ ボタントグル表示系");
    ShowButtonDiscription('B', "ミニマクスボックス");
    ShowButtonDiscription('P', "制御点");
    ShowButtonDiscription('F', "一階微分(接線)ベクトル");
    ShowButtonDiscription('S', "二階微分ベクトル");
    ShowButtonDiscription('C', "曲率ベクトル");
    ShowButtonDiscription('N', "法線ベクトル");
}

// ボタンの説明を表示します
void ShowButtonDiscription(char button, string disc)
{
    printf("    ");
    printf("%c : ", button);
    printf("%s\n", disc.c_str());
}