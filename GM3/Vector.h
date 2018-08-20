#pragma once

#include <cmath>
#include <iostream>

// double型3次元ベクトルクラス
struct Vector3d
{
    double X, Y, Z;

    constexpr Vector3d() : X(0), Y(0), Z(0) {} // デフォルトは0初期化
    constexpr Vector3d(double x, double y, double z) : X(x), Y(y), Z(z) {}

    // 表示用
    void Show() const { std::cout << X << " " << Y << " " << Z << std::endl; }

    // 内積
    constexpr double Dot(const Vector3d &other) const
    {
        return (X * other.X) + (Y * other.Y) + (Z * other.Z);
    }

    // 外積
    constexpr Vector3d Cross(const Vector3d &other) const
    {
        return {
            (Y * other.Z) - (Z * other.Y),
            (Z * other.X) - (X * other.Z),
            (X * other.Y) - (Y * other.X)};
    }

    // 長さ
    double Length() const { return sqrt(X * X + Y * Y + Z * Z); }
    constexpr double LengthSquared() const { return Dot(*this); }

    // 絶対値
    double Absolute() const { return Length(); }

    // 距離
    double DistanceFrom(const Vector3d &other) const { return (other - *this).Length(); }

    // 単位化
    Vector3d Normalize() const { return *this / Length(); }

    // 0確認
    constexpr bool IsZero() const { return (X == 0.0) && (Y == 0.0) && (Z == 0.0); }

    // 0ベクトル
    constexpr static Vector3d Zero() { return {0, 0, 0}; }

    // コピーコンストラクタと代入演算はデフォルトを使用

#pragma region operator

    constexpr Vector3d operator+() const
    {
        return *this;
    }                                                             // 単項+
    constexpr Vector3d operator-() const { return {-X, -Y, -Z}; } // 単項-
    // 和
    constexpr Vector3d operator+(const Vector3d &other) const
    {
        return {X + other.X, Y + other.Y, Z + other.Z};
    }
    // 差
    constexpr Vector3d operator-(const Vector3d &other) const
    {
        return {X - other.X, Y - other.Y, Z - other.Z};
    }
    // 定数倍
    constexpr Vector3d operator*(double value) const
    {
        return {X * value, Y * value, Z * value};
    }
    constexpr Vector3d operator/(double value) const
    {
        return {X / value, Y / value, Z / value};
    }
    friend constexpr Vector3d operator*(double value, const Vector3d &v);
    // 外積
    constexpr Vector3d operator*(const Vector3d &other) const
    {
        return this->Cross(other);
    }
    // 複合代入演算
    Vector3d &operator+=(const Vector3d &other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }
    Vector3d &operator-=(const Vector3d &other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        return *this;
    }
    Vector3d &operator*=(double value)
    {
        X *= value;
        Y *= value;
        Z *= value;
        return *this;
    }
    Vector3d &operator/=(double value)
    {
        X /= value;
        Y /= value;
        Z /= value;
        return *this;
    }

#pragma endregion
};

constexpr Vector3d operator*(double value, const Vector3d &v)
{
    return {value * v.X, value * v.Y, value * v.Z};
}
