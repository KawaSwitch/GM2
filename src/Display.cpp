#define _USE_MATH_DEFINES

#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Background.h"
#include "Axis.h"
#include "ControlPoint.h"
#include "BsplineCurve.h"
#include "Test.h"
#include "Picking.h"
#include "Camera.h"
#include "glUtil.h"
#include "Initialize.h"

NormalAxis* axis; // 軸
GeoGrid2D* grid; // グリッド
Scene* scene; // シーン
extern Scene* test_scene;
extern const int grid_length;

static Box coverBox; // 全体のボックス
static Vector3d rotateCenter; // 回転中心
static bool isFirst = true;

// --- プロトタイプ宣言 ---
void SetRotateCenter();
void ShowRotateCenter(bool isRotating);
void UpdateLookAtZ(const Box* const box);
// ------------------------

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // リクエストがあれば表示を初期位置に戻す
  if (isViewInitRequested)
    {
      InitQuaternion(); // 回転姿勢を初期化
      dist_X = dist_Y = 0.0; // 移動を初期化(Z方向を除く)

      glutPostRedisplay();
      isViewInitRequested = false;
    }

  glEnable(GL_STENCIL_TEST); // ステンシル有効化

  // 1. 背景描画
  glDisable(GL_DEPTH_TEST);
  DrawBackground(BackgroundNormal());
  glEnable(GL_DEPTH_TEST);

  // 2.軸描画
  {
    // 軸の型取り
    glStencilFunc(GL_ALWAYS, static_cast<int>(StencilRef::Axis), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // 軸用ビューポートサイズを計算
    int axisViewWidth = width / 5;
    int axisViewHeight = height / 5;
    int axisViewSize = (axisViewWidth < axisViewHeight) ? axisViewWidth : axisViewHeight;

    // 軸用ビューポートを設定
    double axisLength = axis->GetLength();
    glViewport(axisViewSize / 10, axisViewSize / 10, axisViewSize, axisViewSize);

    // 軸用に変換行列を指定する
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
            -axisLength, axisLength,
            -axisLength, axisLength,
            -axisLength, axisLength);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 軸用ライトのみオンにする
    for (const auto& light : modelLight)
      light->Off();
    for (const auto& light : axisLight)
      light->On();

    glPushMatrix();

    glMultMatrixd(rot_mat); // 回転量はモデルと等しく

    if (axisShowType != AxisShowType::None)
      axis->Draw();

    glPopMatrix();
  }


  // ビューポートをウィンドウ全体に設定
  glViewport(0, 0, width, height);

  // 変換行列を全体用に指定しなおす
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(PersParam::fovy, (GLdouble)width / height, PersParam::zNear, PersParam::zFar);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(
	    0.0, 0.0, 0.0,  // 視点位置
	    0.0, 0.0, -1.0,  // 注視位置
	    0.0, 1.0, 0.0   // 上方向 : y
	    );

  // 形状用ライトのみオンにする
  for (const auto& light : axisLight)
    light->Off();
  for (const auto& light : modelLight)
    light->On();


  // 3. 形状描画
  {
    // 形状の型取り
    // 軸を除いて描画
    glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glPushMatrix();
    {
      // 起動最初の描画で回転中心をウィンドウ中心にする
      if (isFirst)
	{
	  TestRegister(); // 事前に登録しておく 

	  SetRotateCenter();
	  UpdateLookAtZ(&coverBox); // フィット
	}

      glTranslated(-rotateCenter.X, -rotateCenter.Y, -rotateCenter.Z);

      glTranslated(dist_X, dist_Y, dist_Z); // 移動

      // 回転中心を指定して回転
      RotateAt(rot_mat, rotateCenter);

      // テスト描画
      TestDraw();
      // 形状描画
      //scene->Draw();

      // 回転中心描画
      ShowRotateCenter(rotate_flag);

      // 4.グリッド描画
      {
	// グリッドの型取り
	// 上の全部が描画されていない所にグリッドを描画
	glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Grid), 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// 幾何グリッド描画
	if (gridType != GridType::InVisible)
	  grid->Draw();
      }
    }
    glPopMatrix();
  }

  glDisable(GL_STENCIL_TEST); // ステンシル無効化

  isFirst = false;
  glutSwapBuffers();
}

// 回転中心を設定します
void SetRotateCenter()
{
  vector<Box> sceneBoxes;

  // 回転中心を決める
  // すべての形状の中心を回転中心とする
  // TODO: いつか画面中心で回転させたい
  sceneBoxes.push_back(scene->GetCoverBound());
  sceneBoxes.push_back(test_scene->GetCoverBound());
  coverBox = Box(sceneBoxes);

  // とりあえず
  if (coverBox.IsValid() == false)
    {
      Error::ShowMessage(
			 "バウンドボックス設定エラー",
			 "描画形状が設定されていません. 既定の回転ボックス, 回転中心を設定します");
      coverBox = Box(
		     -grid_length, -grid_length, -grid_length,
		     grid_length, grid_length, grid_length);
    }

  auto center = coverBox.Center();

  rotateCenter = Vector3d(center.X, center.Y, center.Z);
}
// 回転中心を表示します
void ShowRotateCenter(bool isRotating)
{
  if (rotate_flag)
    {
      // ライティングは切っておく
      if (glIsEnabled(GL_LIGHTING))
	glDisable(GL_LIGHTING);

      // 中心点描画 デプス値は評価しない
      glColor4dv(Color::orange);
      glPointSize(10.0);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::orange);

      glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::RotateCenter), 0xFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

      glDisable(GL_DEPTH_TEST);
      glBegin(GL_POINTS);
      glVertex3d(rotateCenter);
      glEnd();
      glEnable(GL_DEPTH_TEST);

      // カバーボックス描画  形状の後ろにあれば点線表示
      glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      coverBox.Draw(Color::orange, 2.0); // 普通に描画

      // 陰線は破線表示
      {
	glStencilFunc(GL_GREATER, static_cast<int>(StencilRef::HiddenLine), 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
	coverBox.Draw(Color::orange, 2.0); // 陰線判定用

	glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::HiddenLine), 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0);

	// デプス値は評価しない
	glDisable(GL_DEPTH_TEST);
	coverBox.Draw(Color::orange, 1.5);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_LINE_STIPPLE);
      }

      glPointSize(1.0);
    }
}

// 視点のZ位置をボックスから決定し更新します
void UpdateLookAtZ(const Box* const box)
{
  double boxHalfX = (box->MaxX() - box->MinX()) / 2;
  double boxHalfY = (box->MaxY() - box->MinY()) / 2;
  double boxHalfZ = (box->MaxZ() - box->MinZ()) / 2;
  double ratio; // 仰角に対する縦横比倍率

  // ウィンドウにぴったり収まるような縦横比倍率を場合分け
  if (width > height)
    ratio = boxHalfX / boxHalfY;
  else
    ratio = (double)width / height;

  // ボックス前面からカメラまでの距離
  double distToBoxFront = boxHalfX / std::tan(ratio * (PersParam::fovy / 2) * (M_PI / 180));

  // カメラは原点に置いたままワールドを反対方向に動かす
  double cameraZ = boxHalfZ + distToBoxFront;
  dist_Z = -(cameraZ + boxHalfZ / 2); // ちょっと余裕を持たせておく
}

// コンソールに説明を表示します
void ShowConsoleDiscription()
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
  ShowButtonDiscription("A", "軸表示変更");
  ShowButtonDiscription("G", "グリッド表示変更");
  ShowButtonDiscription("I", "回転/移動(奥行以外)を元に戻す");
  //ShowButtonDiscription("L", "ライティングトグル");

  puts("\n");

  puts("デバグ用出力↓\n");
}

// ボタンの説明を表示します
void ShowButtonDiscription(const char* button, const char* disc)
{
  printf("    ");
  printf("%s : ", button);
  printf("%s\n", disc);
}
