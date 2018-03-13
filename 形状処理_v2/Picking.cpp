#include "GV.h"
#include "Scene.h"
#include "Picking.h"

extern Scene* scene;

// ローカル座標のデプス値を取得する
float GetDepth(int x, int y)
{
    float z;
    GLint viewport[4];  // ビューポート
                        
    // デバイス座標系とウィンドウ座標系の変換
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadBuffer(GL_BACK);

    glReadPixels(x, viewport[3] - y - 1, 1, 1,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        &z);

    return z;
}

// ローカル座標からワールド座標を取得する
Point3d GetWorldCoord(int x, int y, float depth)
{
    double wx, wy, wz;
    GLdouble mvMatrix[16], pjMatrix[16];
    GLint viewport[4];

    // パラメータ取得
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);

    // 世界座標を取得する
    gluUnProject((double)x, (double)viewport[3] - y - 1, depth,
        mvMatrix,
        pjMatrix,
        viewport,
        &wx,
        &wy,
        &wz);

    return Point3d(wx, wy, wz);
}

// ワールド座標からローカル座標を取得する
Point3d GetLocalCoord(int x, int y, int z)
{
    double winX, winY, depth;
    GLdouble mvMatrix[16], pjMatrix[16];
    GLint viewport[4];

    // パラメータ取得
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);

    // 世界座標を取得する
    gluProject((double)x, (double)viewport[3] - y - 1, z,
        mvMatrix,
        pjMatrix,
        viewport,
        &winX,
        &winY,
        &depth);

    return Point3d(winX, winY, depth);
}

// マウスの座標と重なっているオブジェクトの識別番号を返す
unsigned int GetObjNumberOnMousePointer(int x, int y)
{
    GLuint selectBuf[128] = { 0 };  // セレクションバッファ
    GLuint hits; // ヒットナンバー
    GLint viewport[4];  // ビューポート
    unsigned int objNum = 0;

    // セレクション開始

    // 現在のビューポートを取得
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // セレクション用バッファをOpenGLへ渡す
    glSelectBuffer(BUF_MAX, selectBuf);
    
    // OpenGL描画モードをセレクションモードへ
    (void)glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    // 投影変換行列を操作対象とし、現在の投影変換行列をスタックへ入れる
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    // 投影行列を初期化
    glLoadIdentity();

    // マウスピッキングの範囲の設定
    gluPickMatrix(x,
        viewport[3] - y, // ウィンドウ左下を原点としたウィンドウy座標を与える
        100.0, // ピクセル単位のセレクション範囲 XとY
        100.0, // お好みで調整
        viewport);

    glMatrixMode(GL_MODELVIEW);
    // ここでglLoadNameつきで描画して名前をつける
    // 階層の深さは1にすること！
    scene->DrawForPick();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // レンダーモードへ戻す
    hits = glRenderMode(GL_RENDER); // ヒットレコードの戻り値

    // 一番手前のオブジェクトを返す
    if (hits > 0)
        objNum = GetNearestNumber(hits, selectBuf);

    glMatrixMode(GL_MODELVIEW);        //モデルビューモードへ戻す

    // 見つからなければ0を返す
    return objNum;
}

// 階層1として選択データを整理（freeし忘れないで, 後でスマートポインタ実装）
void GetSelectionData(GLuint hits, GLuint* buf, SelectionData* datas)
{
    // 階層1としてデータを整理
    for (unsigned int i = 0, dataI = 0; i < 128; dataI++)
    {
        if (buf[i++] == 0)
            break;

        datas[dataI].minZ = buf[i++];
        datas[dataI].maxZ = buf[i++];
        datas[dataI].name = buf[i++];
    }
}

// もっとも手前にあるオブジェクト番号を取得
unsigned int GetNearestNumber(GLuint hits, GLuint* buf)
{
    SelectionData* data = (SelectionData *)malloc(sizeof(SelectionData) * hits);
    GetSelectionData(hits, buf, data);

    if (data == NULL)
        return 0;

    SelectionData nearest = data[0];

    for (unsigned int i = 1; i < hits; i++)
    {
        if (data[i].minZ < nearest.minZ)
            nearest = data[i];
    }

    auto nearest_num = nearest.name;

    free(data);
    return nearest_num;
}

// もっとも奥にあるオブジェクト番号を取得
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf)
{
    SelectionData* data = (SelectionData *)malloc(sizeof(SelectionData) * hits);
    GetSelectionData(hits, buf, data);

    if (data == NULL)
        return 0;

    SelectionData farthest = data[0];

    for (unsigned int i = 1; i < hits; i++)
    {
        if (data[i].maxZ > farthest.maxZ)
            farthest = data[i];
    }

    free(data);
    return farthest.name;
}