#pragma once

#include "GV.h"
#include "Object.h"

// ���̕\���^�C�v
enum AxisShowType
{
    None,  // �\�����Ȃ�
    Normal, // ���ʂɕ`��
    WithLabel, // XYZ���x�����`��
};

extern AxisShowType axisShowType;

class Axis
{
private:
    // ��󕔕��̐�߂銄��
    const double _ratio = (double)1 / 3;

protected:
    double _length; 
    double _bodyLength; // ���̕����̒���
    double _arrowLength; // ��󕔕��̒���

public:
    Axis(double len) 
    {
        _length = len;

        _bodyLength = len * (1 - _ratio);
        _arrowLength = len * _ratio;
    }

    // ��󕔕����܂߂�������Ԃ�
    double GetLength() { return _length; }
};

// �\���b�h��
class NormalAxis : public Axis
{
private:
    void DrawInternal();
    void DrawCharacter();

public:
    NormalAxis(double len) : Axis(len) { }
    void Draw();
};