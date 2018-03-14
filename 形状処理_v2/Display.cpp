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

    // リクエストがあれば表示を初期位置に戻す
    if (isViewInitRequested)
    {
        InitQuaternion(); // 回転姿勢を初期化
        dist_X = dist_Y = dist_Z = 0.0; // 移動を初期化

        glutPostRedisplay();
        isViewInitRequested = false;
    }

    glEnable(GL_STENCIL_TEST); // ステンシル有効化

    // 1.軸描画
    // TODO: ビューポート別
    if (isShowAxis)
    {
        // 軸の型取り
        glStencilFunc(GL_ALWAYS, static_cast<int>(StencilRef::Axis), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glPushMatrix();
        glScaled(1.3, 1.3, 1.3);
        glTranslated(-1.6, -1.6, -1.6);
        glMultMatrixd(rot_mat);
        axis->Draw();
        glPopMatrix();
    }

    // 2. 形状描画
    glPushMatrix();

    // 形状の型取り
    // 軸を除いて描画
    glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glTranslated(dist_X, -dist_Y, dist_Z); // 移動

    glMultMatrixd(rot_mat); // 回転

    // 形状描画
    scene->Draw();
    // テスト描画
    TestDraw();


    // 3. グリッド描画
    
    // グリッドの型取り
    // 上の全部が描画されていない所にグリッドを描画
    glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Grid), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // 幾何グリッド描画
    DrawGrid(GeoGrid2D());

    glPopMatrix();


    // 4. 背景描画
    glPushMatrix();

    // 上の全部が描画されていない箇所は背景を描画
    glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::Background), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    DrawBackground(BackgroundNormal());

    glPopMatrix();

    glDisable(GL_STENCIL_TEST); // ステンシル無効化

    // --------

    glutSwapBuffers();
}

// コンソールに説明を表示します
void ConsoleDiscription()
{
    puts("形状処理C++ 操作方法\n");

    puts("$ ファイル起動時読み込み");
    puts("./KJS_FILE内で名前の先頭に@をつけた.kjsファイルを起動時に読み込みます\n");

    puts("$ マウス操作");
    ShowButtonDiscription("右ドラッグ", "回転");
    ShowButtonDiscription("中ドラッグ", "移動");
    ShowButtonDiscription("ホイール", "拡大/縮小");

    puts("");

    // エンティティ操作ボタン説明
    puts("$ エンティティトグル表示系");
    ShowButtonDiscription("B", "ミニマクスボックス");
    ShowButtonDiscription("P", "制御点");
    ShowButtonDiscription("F", "一階微分(接線)ベクトル");
    ShowButtonDiscription("S", "二階微分ベクトル");
    ShowButtonDiscription("C", "曲率ベクトル");
    ShowButtonDiscription("N", "法線ベクトル");

    puts("");

    puts("$ その他表示系");
    ShowButtonDiscription("ESC", "終了");
    ShowButtonDiscription("A", "軸表示トグル");
    ShowButtonDiscription("I", "回転/移動を元に戻す");
    //ShowButtonDiscription("L", "ライティングトグル");

    puts("\n");

    puts("デバグ用出力↓\n");
}

// ボタンの説明を表示します
void ShowButtonDiscription(char* button, char* disc)
{
    printf("    ");
    printf("%s : ", button);
    printf("%s\n", disc);
}