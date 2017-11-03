#pragma once
#pragma comment(lib,"glew32.lib")

#include "GV.h"
#include "Scene.h"
#include "Error.h"
#include "ControlPoint.h"
#include "Box.h"

class Scene;

// オブジェクト基底クラス
class Object
{
private:

    bool _deleteFlag = false;
    bool _isSelected = false; // 選択状態にあるか
    unsigned int _number; // 識別番号

protected:

    bool _isUseVBO = false; // VBOを使うか
    GLuint _vbo = 0;
    GLdouble _color[4];  // 色
    vector<ControlPoint> _ctrlp; // 制御点

    // 事前描画
    virtual void PreDraw() = 0;
    virtual void DrawCPsInternal() = 0;
    virtual void DrawFirstDiffVectorsInternal() { };
    virtual void DrawSecondDiffVectorsInternal() { };
    virtual void DrawCurvatureVectorsInternal() { };

    // VBO
    virtual void CreateVBO() { };
    virtual void ModifyVBO() { };
    virtual void DrawVBO() { };
    
    // ディスプレイリスト
    int _displayList = -1; // オブジェクト用
    int _ctrlp_displayList = -1;
    int _fd_displayList = -1;
    int _sd_displayList = -1;
    int _box_displayList = -1;
    int _cur_displayList = -1;

    // 表示系ブーリアン
    bool _isDrawCtrlp = true; // 制御点
    bool _isDrawFirstDiff = false; // 接線
    bool _isDrawSecondDiff = false; // 2階微分
    bool _isDrawBox = false; // ミニマクスボックス
    bool _isDrawCurvature = false; // 曲率

public:

    // オブジェクト描画
    virtual void Draw()
    {
        // VBO
        if (_isUseVBO)
        {
            if (_vbo == 0)
            {
                CreateVBO(); // VBO作成
                glutPostRedisplay();
            }
            else
            {
                glColor3dv(_color); // 色指定
                DrawVBO(); // 描画
            }

        }
        // ディスプレイリスト
        else
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
    }

    // そのまま描画
    void DrawAsItIs()
    {
        this->PreDraw();
    }

    // ミニマクスボックス描画
    void DrawBoxInternal()
    {
        Box box(_ctrlp);
        box.Draw(Color::orange, 1.0);
    }

    // トグルチェンジ
    void SetUnsetSelected() { _isSelected = !_isSelected; }
    void SetUnsetIsDrawCtrlp() { _isDrawCtrlp = !_isDrawCtrlp; }
    void SetUnsetIsDrawFisrtDiff() { _isDrawFirstDiff = !_isDrawFirstDiff; }
    void SetUnsetIsDrawSecondDiff() { _isDrawSecondDiff = !_isDrawSecondDiff; }
    void SetUnsetIsDrawBox() { _isDrawBox = !_isDrawBox; }
    void SetUnsetIsDrawCurvature() { _isDrawCurvature = !_isDrawCurvature; }

    // 制御点線描画
    void DrawControlPointsAndLines()
    {
        if (_isDrawCtrlp)
            DrawUsingDisplayList(&_ctrlp_displayList, [&] { return (*this).DrawCPsInternal(); });
    }
    // 接線ベクトル描画
    void DrawFirstDiffVectors()
    {
        if (_isDrawFirstDiff)
            DrawUsingDisplayList(&_fd_displayList, [&] { return (*this).DrawFirstDiffVectorsInternal(); });
    }
    // 2階微分ベクトル描画
    void DrawSecondDiffVectors()
    {
        if (_isDrawSecondDiff)
            DrawUsingDisplayList(&_sd_displayList, [&] { return (*this).DrawSecondDiffVectorsInternal(); });
    }
    // ミニマクスボックス描画
    void DrawBox()
    {
        if (_isDrawBox)
            DrawUsingDisplayList(&_box_displayList, [&] { return (*this).DrawBoxInternal(); });
    }
    // 曲率ベクトル描画
    void DrawCurvatureVectors()
    {
        if (_isDrawCurvature)
            DrawUsingDisplayList(&_cur_displayList, [&] { return (*this).DrawCurvatureVectorsInternal(); });
    }

    // 色設定
    void SetColor(GLdouble* color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // オブジェクト番号を取得
    unsigned int GetObjectNumber()
    {
        return _number;
    }

    // deleteフラグ
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    Object()
    {
        _number = obj_number++; // 識別子を振る

        _vbo = 0;
        _color[0] = -1;
    }

    virtual ~Object()
    {
        obj_number--; // 識別子を空ける

        // ディスプレイリスト破棄
        glDeleteLists(_displayList, 1);
        glDeleteLists(_ctrlp_displayList, 1);
        glDeleteLists(_fd_displayList, 1);
        glDeleteLists(_sd_displayList, 1);
        glDeleteLists(_box_displayList, 1);
        glDeleteLists(_cur_displayList, 1);

        // VBO破棄
        glDeleteBuffers(1, &_vbo);
    }
};
