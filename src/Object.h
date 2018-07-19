#pragma once
#pragma comment(lib,"glew32.lib")

#include "GV.h"
#include "CalcUtil.h"
#include "Scene.h"
#include "Error.h"
#include "ControlPoint.h"
#include "Box.h"
#include "DrawUtil.h"

class Scene;

// �I�u�W�F�N�g���N���X
class Object
{
private:

    bool _deleteFlag = false;
    bool _isSelected = false; // �I����Ԃɂ��邩
    unsigned int _number; // ���ʔԍ�

protected:

    bool _isUseVBO = false; // VBO���g����
    bool _isUseIBO = false; // IBO���g����
    mutable GLuint _vbo, _ibo;

    GLdouble _color[4];  // �F
    vector<ControlPoint> _ctrlp; // ����_
    double _resolution; // �𑜓x

    // ���O�`��
    virtual void PreDraw() const = 0;
    virtual void DrawCPsInternal() const = 0;
    virtual void DrawFirstDiffVectorsInternal() const { };
    virtual void DrawSecondDiffVectorsInternal() const { };
    virtual void DrawNormalVectorsInternal() const { };
    virtual void DrawCurvatureVectorsInternal() const { };

    // VBO
    virtual void CreateVBO() const { };
    virtual void ModifyVBO() const { };
    virtual void DrawVBO() const { };

    // IBO
    virtual void CreateIBO() const { };
    virtual void ModifyIBO() const { };
    virtual void DrawIBO() const { };
    
    // �f�B�X�v���C���X�g
    mutable int _displayList = 0; // �I�u�W�F�N�g�p
    mutable int _ctrlp_displayList = 0;
    mutable int _fd_displayList = 0;
    mutable int _sd_displayList = 0;
    mutable int _box_displayList = 0;
    mutable int _nor_displayList = 0;
    mutable int _cur_displayList = 0;

    // �\���n�u�[���A��
    bool _isDrawCtrlp = false; // ����_
    bool _isDrawFirstDiff = false; // �ڐ�
    bool _isDrawSecondDiff = false; // 2�K����
    bool _isDrawBox = false; // �~�j�}�N�X�{�b�N�X
    bool _isDrawNormal = false; // �@��
    bool _isDrawCurvature = false; // �ȗ�

public:

    // �I�u�W�F�N�g�`��
    virtual void Draw() const
    {
        // VBO
        if (_isUseVBO)
        {
            if (_vbo == 0)
            {
                CreateVBO(); // VBO�쐬
                glutPostRedisplay();
            }
            else
                DrawVBO(); // �`��
        }
        // IBO(������) �� VBO�ƈꏏ�ɍ�邩��Ⴄ
        else if (_isUseIBO)
        {
            if (_ibo == 0)
            {
                CreateIBO(); // IBO�쐬
                glutPostRedisplay();
            }
            else
                DrawIBO(); // �`��
        }
        // �f�B�X�v���C���X�g
        else
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
    }

    // ���̂܂ܕ`��
    void DrawAsItIs() const
    {
        this->PreDraw();
    }
    void DrawAsItIsWithCPs() const
    {
        this->PreDraw();
        this->DrawCPsInternal();
    }

    // �~�j�}�N�X�{�b�N�X�`��
    void DrawBoxInternal() const
    {
        Box box(_ctrlp);
        box.Draw(Color::light_blue, 1.0);
    }

    // �g�O���`�F���W
    void SetUnsetSelected() { _isSelected = !_isSelected; }
    void SetUnsetIsDrawCtrlp() { _isDrawCtrlp = !_isDrawCtrlp; }
    void SetUnsetIsDrawFisrtDiff() { _isDrawFirstDiff = !_isDrawFirstDiff; }
    void SetUnsetIsDrawSecondDiff() { _isDrawSecondDiff = !_isDrawSecondDiff; }
    void SetUnsetIsDrawBox() { _isDrawBox = !_isDrawBox; }
    void SetUnsetIsDrawNormal() { _isDrawNormal = !_isDrawNormal; }
    void SetUnsetIsDrawCurvature() { _isDrawCurvature = !_isDrawCurvature; }

    // ����_���`��
    void DrawControlPointsAndLines() const
    {
        if (_isDrawCtrlp)
            DrawUsingDisplayList(&_ctrlp_displayList, [&] { return (*this).DrawCPsInternal(); });
    }
    // �ڐ��x�N�g���`��
    void DrawFirstDiffVectors() const
    {
        if (_isDrawFirstDiff)
            DrawUsingDisplayList(&_fd_displayList, [&] { return (*this).DrawFirstDiffVectorsInternal(); });
    }
    // 2�K�����x�N�g���`��
    void DrawSecondDiffVectors() const
    {
        if (_isDrawSecondDiff)
            DrawUsingDisplayList(&_sd_displayList, [&] { return (*this).DrawSecondDiffVectorsInternal(); });
    }
    // �~�j�}�N�X�{�b�N�X�`��
    void DrawBox() const
    {
        if (_isDrawBox)
            DrawUsingDisplayList(&_box_displayList, [&] { return (*this).DrawBoxInternal(); });
    }
    // �@���x�N�g���`��
    void DrawNormalVectors() const
    {
        if (_isDrawNormal)
            DrawUsingDisplayList(&_nor_displayList, [&] { return (*this).DrawNormalVectorsInternal(); });
    }
    // �ȗ��x�N�g���`��
    void DrawCurvatureVectors() const
    {
        if (_isDrawCurvature)
            DrawUsingDisplayList(&_cur_displayList, [&] { return (*this).DrawCurvatureVectorsInternal(); });
    }

    // �F�ݒ�
    void SetColor(const GLdouble* const color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // �������I�u�W�F�N�g�ł��邩
    bool IsSemiTransparent()
    {
        if (fabs(_color[3] - 1.0) < EPS::DIST)
            return false;
        else
            return true;
    }

    // �I�u�W�F�N�g�ԍ����擾
    unsigned int GetObjectNumber() const
    {
        return _number;
    }

    // delete�t���O
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    // �~�j�}�N�X�{�b�N�X�擾
    Box GetBound() const
    {
        Box box(_ctrlp);
        return box;
    }

    Object()
    {
        _number = obj_number++; // ���ʎq��U��

        _vbo = 0;
        _ibo = 0;
        _color[0] = -1;
    }

    virtual ~Object()
    {
        obj_number--; // ���ʎq���󂯂�

        // �f�B�X�v���C���X�g�j��
        glDeleteLists(_displayList, 1);
        glDeleteLists(_ctrlp_displayList, 1);
        glDeleteLists(_fd_displayList, 1);
        glDeleteLists(_sd_displayList, 1);
        glDeleteLists(_box_displayList, 1);
        glDeleteLists(_cur_displayList, 1);

        // VBO�j��
        glDeleteBuffers(1, &_vbo);

        // IBO�j��
        glDeleteBuffers(1, &_ibo);
    }
};
