#pragma once

#include "Object.h"
#include "ControlPoint.h"

// 曲面インターフェース
class Surface : public Object
{
protected:

    int _ordU, _ordV; // 階数
    int _ncpntU, _ncpntV; // 制御点数
    vector<ControlPoint> _ctrlp; // 制御点
    vector<double> _ctrlpX; // 計算用
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;
    int _mesh_displayList = -1; // メッシュ用ディスプレイリスト

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

        _ctrlp.reserve(size); _ctrlpX.reserve(size);
        _ctrlpY.reserve(size); _ctrlpZ.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);

        // 計算用に転置する(もっと見栄えがいいやり方が欲しい)
        for (int i = 0; i < _ncpntU; i++)
        {
            for (int j = 0; j < _ncpntV; j++)
            {
                _ctrlpX.push_back(_ctrlp[j * _ncpntU + i].X);
                _ctrlpY.push_back(_ctrlp[j * _ncpntU + i].Y);
                _ctrlpZ.push_back(_ctrlp[j * _ncpntU + i].Z);
            }
        }
    }

    // メッシュ表示
    virtual void DrawMeshInternal() { };

    void DrawMesh()
    {
        // メッシュは絶対表示(見栄えのため)
        DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); });
    }

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double u, double v) = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVectorU(double u, double v) = 0; // 接線ベクトル
    virtual Vector3d GetFirstDiffVectorV(double u, double v) = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) { return Vector3d(); }; // 2階微分ベクトル
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetCurvatureVector(double u, double v) { return Vector3d(); }; // 曲率ベクトル

public:

    // オブジェクト描画
    void Draw() override
    {
        // オブジェクト自身はデプス考慮無し(見栄えのため)
        glDepthMask(GL_FALSE);

        if (_isUseVBO)
        {
            // VBO
            if (_vbo == 0)
            {
                CreateVBO(); // VBO作成
                glutPostRedisplay();
            }
            else
                DrawVBO(); // 描画
        }
        else
        {
            // ディスプレイリスト
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
        }

        glDepthMask(GL_TRUE);

        // メッシュはディスプレイリスト
        DrawMesh();
    }

    // 制御点描画
    void DrawCPsInternal() override
    {
        glColor3d(1.0, 0.0, 0.0); // 赤
        glPointSize(5.0);
        glLineWidth(1.0);

        // 点群
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // U方向線群
        for (int i = 0; i < _ncpntV; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntU; j++)
                glVertex3d(_ctrlp[i * _ncpntU + j].X, _ctrlp[i * _ncpntU + j].Y, _ctrlp[i * _ncpntU + j].Z);
            glEnd();
        }
        // V方向線群
        for (int i = 0; i < _ncpntU; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntV; j++)
                glVertex3d(_ctrlp[j * _ncpntU + i].X, _ctrlp[j * _ncpntU + i].Y, _ctrlp[j * _ncpntU + i].Z);
            glEnd();
        }
    }

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};