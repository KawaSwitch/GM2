#pragma once

#include <cmath>
#include <iostream>

// double�^3�����x�N�g���N���X
struct Vector3d
{
    double X, Y, Z;

    constexpr Vector3d() : X(0), Y(0), Z(0) { } // �f�t�H���g��0������
    constexpr Vector3d(double x, double y, double z) : X(x), Y(y), Z(z) { }

    // �\���p
    void Show() const { std::cout << X << " " << Y << " " << Z << std::endl; }

    // ����
    constexpr double Dot(const Vector3d& other) const 
    {
        return
            (X * other.X) + (Y * other.Y) + (Z * other.Z);
    }

    // �O��
    constexpr Vector3d Cross(const Vector3d& other) const
    {
        return {
        (Y * other.Z) - (Z * other.Y),
        (Z * other.X) - (X * other.Z),
        (X * other.Y) - (Y * other.X) };
    }

    // ����
    double Length() const { return sqrt(X*X + Y*Y + Z*Z); }
    constexpr double LengthSquared() const { return Dot(*this); }
    
    // ��Βl
    double Absolute() const { return Length(); }

    // ����
    double DistanceFrom(const Vector3d& other) const { return (other - *this).Length(); }

    // �P�ʉ�
    Vector3d Normalize() const { return *this / Length(); }

    // 0�m�F
    constexpr bool IsZero() const { return (X == 0.0) && (Y == 0.0) && (Z == 0.0); }

    // 0�x�N�g��
    constexpr static Vector3d Zero() { return { 0, 0, 0 }; }

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�̓f�t�H���g���g�p


#pragma region operator

    constexpr Vector3d operator + () const { return *this; } // �P��+
    constexpr Vector3d operator - () const { return { -X, -Y, -Z }; } // �P��-
    // �a
    constexpr Vector3d operator+(const Vector3d& other) const
    {
        return { X + other.X, Y + other.Y, Z + other.Z };
    }
    // ��
    constexpr Vector3d operator-(const Vector3d& other) const
    {
        return { X - other.X, Y - other.Y, Z - other.Z };
    }
    // �萔�{
    constexpr Vector3d operator*(double value) const
    {
        return { X * value, Y * value, Z * value };
    }
    constexpr Vector3d operator/(double value) const
    {
        return { X / value, Y / value, Z / value };
    }
    friend 
        constexpr Vector3d operator*(double value, const Vector3d& v);
    // �O��
    constexpr Vector3d operator*(const Vector3d& other) const
    {
        return this->Cross(other);
    }
    // ����������Z
    Vector3d& operator+=(const Vector3d& other)
    {
        X += other.X; Y += other.Y; Z += other.Z;
        return *this;
    }
    Vector3d& operator-=(const Vector3d& other)
    {
        X -= other.X; Y -= other.Y; Z -= other.Z;
        return *this;
    }
    Vector3d& operator*=(double value)
    {
        X *= value; Y *= value; Z *= value;
        return *this;
    }
    Vector3d& operator/=(double value)
    {
        X /= value; Y /= value; Z /= value;
        return *this;
    }

#pragma endregion
};

constexpr Vector3d operator*(double value, const Vector3d& v)
{
    return { value * v.X, value * v.Y, value * v.Z };
}