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

public:

    // 描画
    virtual void Draw()
    {
        // VBO
        if (_isUseVBO)
        {
            if (!_isRendered)
            {
                CreateVBO();

                _isRendered = true;

                glutPostRedisplay();
            }
            else
            {
                glBindBuffer(GL_ARRAY_BUFFER, _vbo);
                glVertexPointer(3, GL_DOUBLE, 0, 0);

                glEnableClientState(GL_VERTEX_ARRAY);

                glDrawArrays(GL_LINE_STRIP, 0, 901);

                glDisableClientState(GL_VERTEX_ARRAY);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    // deleteフラグ
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    Object()
    {
        _isRendered = false;
        _deleteFlag = false;

        _color[0] = -1;
    }

    virtual ~Object()
    {
        glDeleteLists(_displayList, 1);
    }
};
