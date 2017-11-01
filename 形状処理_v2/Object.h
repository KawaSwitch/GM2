#pragma once
#pragma comment(lib,"glew32.lib")

#include "GV.h"
#include "Scene.h"
#include "Error.h"

class Scene;

// オブジェクト基底クラス
class Object
{
private:

    int _displayList;    // ディスプレイリスト
    bool _isRendered;
    bool _deleteFlag;
    bool _isSelected = false; // 選択状態にあるか
    unsigned int _number; // 識別番号

    // ディスプレイリスト設定
    void SetDisplayList()
    {
        _displayList = glGenLists(1);

        if (_displayList == 0)
            Error::ShowAndExit("ディスプレイリスト作成失敗");
    }

protected:

    bool _isUseVBO = false; // VBOを使うか
    GLuint _vbo;
    GLdouble _color[4];  // 色

    // 事前描画
    virtual void PreDraw() = 0;
    virtual void CreateVBO() { };
    virtual void ModifyVBO() { };
    virtual void DrawVBO() { };

    bool _isDrawCtrlp = false;

public:

    // 描画
    virtual void Draw()
    {
        // VBO
        if (_isUseVBO)
        {
            if (!_isRendered)
            {
                CreateVBO(); // VBO作成

                _isRendered = true;
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
        {
            if (!_isRendered)
            {
                SetDisplayList();

                // 登録
                glNewList(_displayList, GL_COMPILE);
                this->PreDraw();
                glEndList();

                _isRendered = true;

                glutPostRedisplay();
            }
            else
            {
                glCallList(_displayList);
            }
        }
    }

    // そのまま描画
    void DrawAsItIs()
    {
        this->PreDraw();
    }

    // 色設定
    void SetColor(GLdouble* color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // 選択状態トグル
    void SetUnsetSelected()
    {
        _isSelected = !_isSelected;
    }

    // 制御点線描画
    virtual void DrawControlPointsAndLines() = 0;

    // 制御点表示トグル
    void SetUnsetIsDrawCtrlp()
    {
        _isDrawCtrlp = !_isDrawCtrlp;
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
        _isRendered = false;
        _deleteFlag = false;
        _number = obj_number++; // 識別子を振る

        _color[0] = -1;
    }

    virtual ~Object()
    {
        obj_number--; // 識別子を空ける
        glDeleteLists(_displayList, 1);
    }
};
