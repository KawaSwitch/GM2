#pragma once

#include "EPS.h"

// 制御点クラス
// 同次座標考慮
class ControlPoint
{
  public:
    double X, Y, Z; // 座標
    double W;       // ウェイト

    constexpr ControlPoint() : X(0), Y(0), Z(0), W(1.0) {}
    constexpr ControlPoint(double x, double y, double z) : X(x), Y(y), Z(z), W(1.0) {}
    constexpr ControlPoint(double x, double y, double z, double w) : X(x), Y(y), Z(z), W(w) {}

    operator Vector3d()
    {
        // 通常座標系に戻す
        return Vector3d(X / W, Y / W, Z / W);
    }

    bool operator==(const ControlPoint &other)
    {
        return this->GetDistanceTo(other) < EPS::DIST;
    }

    bool operator!=(const ControlPoint &other)
    {
        return !(*this == other);
    }

    void Show() const
    {
        std::cout << X << " " << Y << " " << Z << " " << W << std::endl;
    }

    double GetDistanceTo(ControlPoint other)
    {
        return sqrt(
            (X - other.X) * (X - other.X) +
            (Y - other.Y) * (Y - other.Y) +
            (Z - other.Z) * (Z - other.Z));
    }

    // ベクトル計算用定数倍
    constexpr Vector3d operator*(double value) const
    {
        return {X * value, Y * value, Z * value};
    }
    constexpr Vector3d operator/(double value) const
    {
        return {X / value, Y / value, Z / value};
    }

    friend constexpr Vector3d operator*(double value, const ControlPoint &cp);
};

constexpr Vector3d operator*(double value, const ControlPoint &cp)
{
    return {value * cp.X, value * cp.Y, value * cp.Z};
}
