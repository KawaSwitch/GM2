#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "Model.h"
#include "Triangle.h"
#include "Reader.h"

extern NormalAxis* axis;
extern Scene* scene;
extern Scene* test_scene;

void InitScene()
{
    scene = new Scene();
    test_scene = new Scene(); // テスト用もここで初期化

    // @ファイル読み込み
    auto reader = new KjsReader("KJS_FILE/");
    auto objs = reader->GetObjectsFromKjsFolder();

    for (unsigned int i = 0; i < objs.size(); i++)
        scene->AddObject(objs[i]);
}

void InitQuaternion()
{
    // 回転行列
    for (int i = 0; i < 16; i++)
        rot_mat[i] = 0.0;

    // クォータニオン
    target.x = 0.0; target.y = 0.0;
    target.z = 0.0; target.w = 0.0;
    current.x = 0.0; current.y = 0.0;
    current.z = 0.0; current.w = 1.0;

    // クォータニオン初期化
    CalcRotateMatrix(rot_mat, current);
}

void Initialize()
{
    glewInit(); // glew拡張

    glClearColor(1.0, 1.0, 1.0, 1.0);   // 背景色：白

    glEnable(GL_DEPTH_TEST); // デプス値 有効化

    glEnable(GL_BLEND); // ブレンド 有効化
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mainLight.InitLight(); // メインライト初期化
    isUseLight = true; // デフォルトはオフ → とりあえずオン(必要なところのみ処理を施す)

    //// カリング処理
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    axis = new NormalAxis(0.4); // 軸

    obj_number = 1; // 初期識別子は1

    // 回転移動系初期化
    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;
    InitQuaternion();

    // シーン初期化
    InitScene();
}