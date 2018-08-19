#pragma once
#pragma comment(lib, "glew32.lib")

#include "GV.h"
#include "CalcUtil.h"
#include "Scene.h"
#include "Error.h"
#include "ControlPoint.h"
#include "Box.h"
#include "DrawUtil.h"

extern bool canUseVbo;

class Scene;

// オブジェクト基底クラス
class Object
{
  private:
    bool _deleteFlag = false;
    bool _isSelected = false;  // 選択状態にあるか
    unsigned int _number;      // 識別番号
    bool _isBoxCalced = false; // ボックスを計算済みか

  protected:
    bool _isUseVBO = canUseVbo; // VBOを使うか
    bool _isUseIBO = false;     // IBOを使うか
    mutable GLuint _vbo, _ibo;

    std::string _name;           // 名前
    vector<ControlPoint> _ctrlp; // 制御点
    double _resolution;          // 解像度
    Box _box;                    // バウンドボックス
    GLdouble _boxColor[4];       // バウンドボックス色(現在はオブジェクト色を使用)
    GLdouble _boxWidth;          // ボックスの太さ

    // 事前描画
    virtual void PreDraw() const = 0;
    virtual void DrawCPsInternal() const = 0;
    virtual void DrawFirstDiffVectorsInternal() const {};
    virtual void DrawSecondDiffVectorsInternal() const {};
    virtual void DrawNormalVectorsInternal() const {};
    virtual void DrawCurvatureVectorsInternal() const {};

    // VBO
    virtual void CreateVBO() const {};
    virtual void ModifyVBO() const {};
    virtual void DrawVBO() const {};

    // IBO
    virtual void CreateIBO() const {};
    virtual void ModifyIBO() const {};
    virtual void DrawIBO() const {};

    // ディスプレイリスト
    mutable int _displayList = 0; // オブジェクト用
    mutable int _ctrlp_displayList = 0;
    mutable int _fd_displayList = 0;
    mutable int _sd_displayList = 0;
    mutable int _box_displayList = 0;
    mutable int _nor_displayList = 0;
    mutable int _cur_displayList = 0;

    // 表示系ブーリアン
    bool _isDrawCtrlp = false;      // 制御点
    bool _isDrawFirstDiff = false;  // 接線
    bool _isDrawSecondDiff = false; // 2階微分
    bool _isDrawBox = false;        // ミニマクスボックス
    bool _isDrawNormal = false;     // 法線
    bool _isDrawCurvature = false;  // 曲率

    // 表示系の識別子をすべてクリアする
    void ClearAllShowingIds()
    {
        // ディスプレイリスト破棄
        glDeleteLists(_displayList, 1);
        glDeleteLists(_ctrlp_displayList, 1);
        glDeleteLists(_fd_displayList, 1);
        glDeleteLists(_sd_displayList, 1);
        glDeleteLists(_box_displayList, 1);
        glDeleteLists(_cur_displayList, 1);

        // VBO破棄
        glDeleteBuffers(1, &_vbo);

        // IBO破棄
        glDeleteBuffers(1, &_ibo);
    }

  public:
    GLdouble _color[4]; // 色

  public:
    // オブジェクト描画
    virtual void Draw() const
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
                DrawVBO(); // 描画
        }
        // IBO(未完成) ← VBOと一緒に作るから違う
        else if (_isUseIBO)
        {
            if (_ibo == 0)
            {
                CreateIBO(); // IBO作成
                glutPostRedisplay();
            }
            else
                DrawIBO(); // 描画
        }
        // ディスプレイリスト
        else
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
    }

    // そのまま描画
    void DrawAsItIs() const
    {
        this->PreDraw();
    }
    void DrawAsItIsWithCPs() const
    {
        this->PreDraw();
        this->DrawCPsInternal();
    }

    // ミニマクスボックス描画
    void DrawBoxInternal() const
    {
        Box box(_ctrlp);
        if (_color[0] == -1)
            box.Draw(Color::light_blue, _boxWidth);
        else
            box.Draw(_color, _boxWidth);
    }
    void SetBoxColor(const GLdouble *const color)
    {
        for (int i = 0; i < 4; i++)
            _boxColor[i] = color[i];
    }
    void SetBoxWidth(const GLdouble width)
    {
        _boxWidth = width;
    }

    // トグルチェンジ
    void SetUnsetSelected() { _isSelected = !_isSelected; }
    void SetUnsetIsDrawCtrlp() { _isDrawCtrlp = !_isDrawCtrlp; }
    void SetUnsetIsDrawFisrtDiff() { _isDrawFirstDiff = !_isDrawFirstDiff; }
    void SetUnsetIsDrawSecondDiff() { _isDrawSecondDiff = !_isDrawSecondDiff; }
    void SetUnsetIsDrawBox() { _isDrawBox = !_isDrawBox; }
    void SetUnsetIsDrawNormal() { _isDrawNormal = !_isDrawNormal; }
    void SetUnsetIsDrawCurvature() { _isDrawCurvature = !_isDrawCurvature; }

    // 制御点線描画
    void DrawControlPointsAndLines() const
    {
        if (_isDrawCtrlp)
            DrawUsingDisplayList(&_ctrlp_displayList, [&] { return (*this).DrawCPsInternal(); });
    }
    // 接線ベクトル描画
    void DrawFirstDiffVectors() const
    {
        if (_isDrawFirstDiff)
            DrawUsingDisplayList(&_fd_displayList, [&] { return (*this).DrawFirstDiffVectorsInternal(); });
    }
    // 2階微分ベクトル描画
    void DrawSecondDiffVectors() const
    {
        if (_isDrawSecondDiff)
            DrawUsingDisplayList(&_sd_displayList, [&] { return (*this).DrawSecondDiffVectorsInternal(); });
    }
    // ミニマクスボックス描画
    void DrawBox() const
    {
        if (_isDrawBox)
            DrawUsingDisplayList(&_box_displayList, [&] { return (*this).DrawBoxInternal(); });
    }
    // 法線ベクトル描画
    void DrawNormalVectors() const
    {
        if (_isDrawNormal)
            DrawUsingDisplayList(&_nor_displayList, [&] { return (*this).DrawNormalVectorsInternal(); });
    }
    // 曲率ベクトル描画
    void DrawCurvatureVectors() const
    {
        if (_isDrawCurvature)
            DrawUsingDisplayList(&_cur_displayList, [&] { return (*this).DrawCurvatureVectorsInternal(); });
    }

    // 色設定
    void SetColor(const GLdouble *const color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // 名前のセット
    void SetName(const std::string name) { _name = name; }
    // 名前の取得
    std::string GetName() const { return _name; }

    // 半透明オブジェクトであるか
    bool IsSemiTransparent()
    {
        if (fabs(_color[3] - 1.0) < EPS::DIST)
            return false;
        else
            return true;
    }

    // オブジェクト番号を取得
    unsigned int GetObjectNumber() const
    {
        return _number;
    }

    // deleteフラグ
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

  public:
    // バウンドボックス取得
    Box GetBound()
    {
        if (!_isBoxCalced)
        {
            Box box(_ctrlp);
            _box = box;
            _isBoxCalced = true;
        }
        return _box;
    }

    Object()
    {
        _number = obj_number++;          // 識別子を振る
        _name = std::to_string(_number); // ここで規定値として振っておく

        _vbo = 0;
        _ibo = 0;
        _color[0] = -1;
        _boxColor[0] = -1;
        _boxWidth = 1;
    }

    virtual ~Object()
    {
        // デストラクタ通知 デバグ用
        //cout << "Calling Object Destructor : " << _number << endl;

        ClearAllShowingIds();
    }
};
