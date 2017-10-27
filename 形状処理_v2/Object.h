#pragma once

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

    // ディスプレイリスト設定
    void SetDisplayList()
    {
        _displayList = glGenLists(1);

        if (_displayList == 0)
            Error::ShowAndExit("ディスプレイリスト作成失敗");
    }

protected:

    GLdouble _color[4];  // 色

    // 事前描画
    virtual void PreDraw() = 0;

public:

    // 描画
    virtual void Draw()
    {
        if (!_isRendered)
        {
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

    // 色設定
    void SetColor(GLdouble* color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // deleteフラグ
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    Object()
    {
        this->SetDisplayList();
        _isRendered = false;
        _deleteFlag = false;

        _color[0] = -1;
    }

    virtual ~Object()
    {
        glDeleteLists(_displayList, 1);
    }
};
