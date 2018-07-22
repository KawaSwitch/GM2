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

// ���֥������ȴ��쥯�饹
class Object
{
private:

    bool _deleteFlag = false;
    bool _isSelected = false; // ������֤ˤ��뤫
    unsigned int _number; // �����ֹ�
    std::string _name;

protected:

    bool _isUseVBO = false; // VBO��Ȥ���
    bool _isUseIBO = false; // IBO��Ȥ���
    mutable GLuint _vbo, _ibo;

    GLdouble _color[4];  // ��
    vector<ControlPoint> _ctrlp; // ������
    double _resolution; // ������

    // ��������
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
    
    // �ǥ����ץ쥤�ꥹ��
    mutable int _displayList = 0; // ���֥���������
    mutable int _ctrlp_displayList = 0;
    mutable int _fd_displayList = 0;
    mutable int _sd_displayList = 0;
    mutable int _box_displayList = 0;
    mutable int _nor_displayList = 0;
    mutable int _cur_displayList = 0;

    // ɽ���ϥ֡��ꥢ��
    bool _isDrawCtrlp = false; // ������
    bool _isDrawFirstDiff = false; // ����
    bool _isDrawSecondDiff = false; // 2����ʬ
    bool _isDrawBox = false; // �ߥ˥ޥ����ܥå���
    bool _isDrawNormal = false; // ˡ��
    bool _isDrawCurvature = false; // ��Ψ

    // ɽ���Ϥμ��̻Ҥ򤹤٤ƥ��ꥢ����
    void ClearAllShowingIds()
    {
      // �ǥ����ץ쥤�ꥹ���˴�
      glDeleteLists(_displayList, 1);
      glDeleteLists(_ctrlp_displayList, 1);
      glDeleteLists(_fd_displayList, 1);
      glDeleteLists(_sd_displayList, 1);
      glDeleteLists(_box_displayList, 1);
      glDeleteLists(_cur_displayList, 1);

      // VBO�˴�
      glDeleteBuffers(1, &_vbo);

      // IBO�˴�
      glDeleteBuffers(1, &_ibo);
    }
    

public:

    // ���֥�����������
    virtual void Draw() const
    {
        // VBO
        if (_isUseVBO)
        {
            if (_vbo == 0)
            {
                CreateVBO(); // VBO����
                glutPostRedisplay();
            }
            else
                DrawVBO(); // ����
        }
        // IBO(̤����) �� VBO�Ȱ��˺�뤫��㤦
        else if (_isUseIBO)
        {
            if (_ibo == 0)
            {
                CreateIBO(); // IBO����
                glutPostRedisplay();
            }
            else
                DrawIBO(); // ����
        }
        // �ǥ����ץ쥤�ꥹ��
        else
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
    }

    // ���Τޤ�����
    void DrawAsItIs() const
    {
        this->PreDraw();
    }
    void DrawAsItIsWithCPs() const
    {
        this->PreDraw();
        this->DrawCPsInternal();
    }

    // �ߥ˥ޥ����ܥå�������
    void DrawBoxInternal() const
    {
        Box box(_ctrlp);
        box.Draw(Color::light_blue, 1.0);
    }

    // �ȥ��������
    void SetUnsetSelected() { _isSelected = !_isSelected; }
    void SetUnsetIsDrawCtrlp() { _isDrawCtrlp = !_isDrawCtrlp; }
    void SetUnsetIsDrawFisrtDiff() { _isDrawFirstDiff = !_isDrawFirstDiff; }
    void SetUnsetIsDrawSecondDiff() { _isDrawSecondDiff = !_isDrawSecondDiff; }
    void SetUnsetIsDrawBox() { _isDrawBox = !_isDrawBox; }
    void SetUnsetIsDrawNormal() { _isDrawNormal = !_isDrawNormal; }
    void SetUnsetIsDrawCurvature() { _isDrawCurvature = !_isDrawCurvature; }

    // ������������
    void DrawControlPointsAndLines() const
    {
        if (_isDrawCtrlp)
            DrawUsingDisplayList(&_ctrlp_displayList, [&] { return (*this).DrawCPsInternal(); });
    }
    // �����٥��ȥ�����
    void DrawFirstDiffVectors() const
    {
        if (_isDrawFirstDiff)
            DrawUsingDisplayList(&_fd_displayList, [&] { return (*this).DrawFirstDiffVectorsInternal(); });
    }
    // 2����ʬ�٥��ȥ�����
    void DrawSecondDiffVectors() const
    {
        if (_isDrawSecondDiff)
            DrawUsingDisplayList(&_sd_displayList, [&] { return (*this).DrawSecondDiffVectorsInternal(); });
    }
    // �ߥ˥ޥ����ܥå�������
    void DrawBox() const
    {
        if (_isDrawBox)
            DrawUsingDisplayList(&_box_displayList, [&] { return (*this).DrawBoxInternal(); });
    }
    // ˡ���٥��ȥ�����
    void DrawNormalVectors() const
    {
        if (_isDrawNormal)
            DrawUsingDisplayList(&_nor_displayList, [&] { return (*this).DrawNormalVectorsInternal(); });
    }
    // ��Ψ�٥��ȥ�����
    void DrawCurvatureVectors() const
    {
        if (_isDrawCurvature)
            DrawUsingDisplayList(&_cur_displayList, [&] { return (*this).DrawCurvatureVectorsInternal(); });
    }

    // ������
    void SetColor(const GLdouble* const color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // ̾���Υ��å�
    void SetName(std::string name) { _name = name; }
    // ̾���μ���
    std::string GetName() const { return _name; };

    // ȾƩ�����֥������ȤǤ��뤫
    bool IsSemiTransparent()
    {
        if (fabs(_color[3] - 1.0) < EPS::DIST)
            return false;
        else
            return true;
    }

    // ���֥��������ֹ�����
    unsigned int GetObjectNumber() const
    {
        return _number;
    }

    // delete�ե饰
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    // �ߥ˥ޥ����ܥå�������
    Box GetBound() const
    {
        Box box(_ctrlp);
        return box;
    }

    Object()
    {
        _number = obj_number++; // ���̻Ҥ򿶤�
	_name = std::to_string(_number); // �����ǵ����ͤȤ��ƿ��äƤ���

        _vbo = 0;
        _ibo = 0;
        _color[0] = -1;
    }

    virtual ~Object()
    {
        obj_number--; // ���̻Ҥ������

	ClearAllShowingIds();
    }
};
