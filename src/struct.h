#pragma once

#include "define.h"

// �N�H�[�^�j�I��
struct Quaternion
{
    double x;
    double y;
    double z;
    double w;

    // ����
    double Length() const { return sqrt(x*x + y*y + z*z + w*w); }

    // �P�ʉ�
    Quaternion Normalize() const { return *this / Length(); }

    // �萔�{
    constexpr Quaternion operator*(double value) const
    {
        return{ x * value, y * value, z * value, w * value };
    }
    constexpr Quaternion operator/(double value) const
    {
        return{ x / value, y / value, z / value, w / value };
    }
};

// 2�����_
template<class T>
struct Point
{
    T x;
    T y;
};

// �Ȗʂ�UV�p�����[�^
enum ParamUV
{
    U,
    V,
};

// �Ȗʂ̒[(�Ȑ�)
enum SurfaceEdge
{
    U_min, // U�p�����[�^���ŏ��̂Ƃ��̃A�C�\�Ȑ�
    U_max, // �ȉ����l
    V_min,
    V_max,
};

// �I���f�[�^
struct SelectionData
{
    unsigned int minZ;
    unsigned int maxZ;
    unsigned int name;

    SelectionData() { minZ = 0; maxZ = 0; name = 0; }
    SelectionData(unsigned int m, unsigned int M, unsigned int n)
    {
        minZ = m;
        maxZ = M;
        name = n;
    }
};

// �X�e���V���Q�ƒl
enum class StencilRef
{
    Background = 0, // �w�i��0(�����l)
    Grid,
    Entity, // �Ȑ��E�Ȗʗp
    HiddenLine, // �A��
    RotateCenter, // ��]���S
    Axis,
    Ignore, // �Ӗ��Ȃ� Keep���Ďg���̂�z��
};

// �O���b�h�̕\���@
enum GridType
{
    NonAxis,
    WithAxis,
    InVisible,
};