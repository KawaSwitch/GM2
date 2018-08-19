#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "GeoGrid.h"
#include "Model.h"
#include "Triangle.h"
#include "Reader.h"
#include "Axis.h"
#include "Initialize.h"

extern NormalAxis* axis;
extern GeoGrid2D* grid;
extern Scene* scene;
extern Scene* test_scene;
extern bool canUseVbo;

const int grid_length = 200;
const int grid_interval = 5;

void InitScene()
{
    scene = new Scene();
    test_scene = new Scene(); // テスト用もここで初期化

    // @ファイル読み込み TODO: ファイル読み込み復活
    // auto reader = new KjsReader("KJS_FILE/");
    // auto objs = reader->GetObjectsFromKjsFolder();

    // for (unsigned int i = 0; i < objs.size(); i++)
    //     scene->AddObject(objs[i]);
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

// OpenGLバージョンによる設定を確認する
void CheckOpenglVersion()
{
    SetCanUseVbo(); // VBOチェック
}
// VBOが使用可能かをセットする
void SetCanUseVbo()
{
    std::string glVersion = (reinterpret_cast<char const *>(glGetString(GL_VERSION)));
    std::cout << "Your OpenGL version: " << glVersion << std::endl;

    // 多分環境依存 1文字目と3文字目にこないかもなので要改良
    int major = (int)glVersion[0] - '0';
    int minor = (int)glVersion[2] - '0';

    // VBOが使用できるのは1.5から
    canUseVbo = (major == 1 && minor >= 5) || major > 1;

    if (!canUseVbo)
        std::cout << "Warning: " << "version1.5未満: 負荷の大きいレンダリングモードで実行します." << std::endl;
    std::cout << std::endl;
}

void Initialize()
{
    glewExperimental = GL_TRUE;
    glewInit(); // glew拡張

    CheckOpenglVersion(); // OpenGLバージョン互換

    glClearColor(1.0, 1.0, 1.0, 1.0);   // 背景色：白
    glClearStencil(0); // ステンシル値は0で初期化

    glEnable(GL_DEPTH_TEST); // デプス値 有効化
    glDepthFunc(GL_LESS);

    glEnable(GL_POINT_SMOOTH); // 点にアンチエイリアシング処理を行う
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // アンチエイリアシング品質を最高に

    glShadeModel(GL_SMOOTH);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND); // ブレンド 有効化

    // メインライト初期化
    {
        lights.push_back(new Light(GL_LIGHT0, 50, 50, -50));
        lights.push_back(new Light(GL_LIGHT1, 50, 0, 50));
        lights.push_back(new Light(GL_LIGHT2, -50, -50, 0));
        lights.push_back(new Light(GL_LIGHT3, 50, 50, 50));
        lights.push_back(new Light(GL_LIGHT4, -50, 0, 0));

        // 軸用ライトの設定
        for (const auto light : lights)
            axisLight.push_back(light);
        // 形状用ライトの設定
        {
            modelLight.push_back(lights[0]);
            modelLight.push_back(lights[1]);
        }

        isUseLight = true; // 必要なところのみ処理を施す
    }

    // グリッド初期化
    gridType = GridType::InVisible;
    grid = new GeoGrid2D(grid_length, grid_interval);

    // 軸初期化
    axisShowType = AxisShowType::Normal;
    axis = new NormalAxis(10); // 軸

    obj_number = 1; // 初期識別子は1

    // 回転移動系初期化
    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;
    InitQuaternion();

    isViewInitRequested = false; // 行列元に戻すか

    // シーン初期化
    InitScene();

    // コンソール出力
    ShowConsoleDiscription();
}
