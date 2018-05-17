#pragma once
#include "Quaternion.h"

// 同時変換行列
struct Matrix3d
{
    double M11, M12, M13, M14;
    double M21, M22, M23, M24;
    double M31, M32, M33, M34;
    double M41, M42, M43, M44;

    // Constructor
    constexpr Matrix3d(
        double m11, double m12, double m13, double m14,
        double m21, double m22, double m23, double m24,
        double m31, double m32, double m33, double m34,
        double m41, double m42, double m43, double m44) 
        : 
        M11(m11), M12(m12), M13(m13), M14(m14),
        M21(m21), M22(m22), M23(m23), M24(m24),
        M31(m31), M32(m32), M33(m33), M34(m34),
        M41(m41), M42(m42), M43(m43), M44(m44) { }

    constexpr Matrix3d()
        :
        M11(1), M12(0), M13(0), M14(0),
        M21(0), M22(1), M23(0), M24(0),
        M31(0), M32(0), M33(1), M34(0),
        M41(0), M42(0), M43(0), M44(1) { }

    // 単位行列の生成
    constexpr static Matrix3d CreateIdentity()
    {
        return
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    // 平行移動
    static void Translate(Matrix3d& matrix, double dx, double dy, double dz)
    {
        matrix.M11 += matrix.M14 * dx; matrix.M12 += matrix.M14 * dy; matrix.M13 += matrix.M14 * dz;
        matrix.M21 += matrix.M24 * dx; matrix.M22 += matrix.M24 * dy; matrix.M23 += matrix.M24 * dz;
        matrix.M31 += matrix.M34 * dx; matrix.M32 += matrix.M34 * dy; matrix.M33 += matrix.M34 * dz;
        matrix.M41 += matrix.M44 * dx; matrix.M42 += matrix.M44 * dy; matrix.M43 += matrix.M44 * dz;
    }
    Matrix3d TranslateX(double dx)
    {
        return Translate(dx, 0, 0);
    }
    Matrix3d TranslateY(double dy)
    {
        return Translate(0, dy, 0);
    }
    Matrix3d TranslateZ(double dz)
    {
        return Translate(0, 0, dz);
    }
    Matrix3d Translate(double dx, double dy, double dz)
    {
        Matrix3d result = *this;
        Translate(result, dx, dy, dz);
        return result;
    }

    // 回転
    static void Rotate(Matrix3d& matrix, Quaternion& quaternion, Matrix3d& result)
    {
        Matrix3d rotation;
        CreateRotation(quaternion, rotation);

        Matrix3d temp;
        Multiply(matrix, rotation, temp);

        result = temp;
    }
    Matrix3d Rotate(Quaternion quaternion)
    {
        Matrix3d result;
        Rotate(*this, quaternion, result);
        return result;
    }
    Matrix3d RotateAt(Quaternion quaternion, Vector3d center)
    {
        Vector3d temp = this->Transform(center);

        Matrix3d result = *this;

        Translate(result, -temp.X, -temp.Y, -temp.Z);
        Rotate(result, quaternion, result);
        Translate(result, temp.X, temp.Y, temp.Z);

        return result;
    }

    // クオータニオンから回転行列の生成
    static void CreateRotation(Quaternion& quaternion, Matrix3d& matrix)
    {
        double x2 = quaternion.x + quaternion.x;
        double y2 = quaternion.y + quaternion.y;
        double z2 = quaternion.z + quaternion.x;
        double xx = quaternion.x * x2;
        double xy = quaternion.x * y2;
        double xz = quaternion.x * z2;
        double yy = quaternion.y * y2;
        double yz = quaternion.y * z2;
        double zz = quaternion.z * z2;
        double wx = quaternion.w * x2;
        double wy = quaternion.w * y2;
        double wz = quaternion.w * z2;

        matrix = Matrix3d::CreateIdentity();

        matrix.M11 = 1.0 - (yy + zz);
        matrix.M12 = xy + wz;
        matrix.M13 = xz - wy;
        matrix.M21 = xy - wz;
        matrix.M22 = 1.0 - (xx + zz);
        matrix.M23 = yz + wx;
        matrix.M31 = xz + wy;
        matrix.M32 = yz - wx;
        matrix.M33 = 1.0 - (xx + yy);
    }

    // 積
    static void Multiply(Matrix3d& left, Matrix3d& right, Matrix3d& result)
    {
        result.M11 = left.M11 * right.M11 + left.M12 * right.M21 + left.M13 * right.M31 + left.M14 * right.M41;
        result.M12 = left.M11 * right.M12 + left.M12 * right.M22 + left.M13 * right.M32 + left.M14 * right.M42;
        result.M13 = left.M11 * right.M13 + left.M12 * right.M23 + left.M13 * right.M33 + left.M14 * right.M43;
        result.M14 = left.M11 * right.M14 + left.M12 * right.M24 + left.M13 * right.M34 + left.M14 * right.M44;
        result.M21 = left.M21 * right.M11 + left.M22 * right.M21 + left.M23 * right.M31 + left.M24 * right.M41;
        result.M22 = left.M21 * right.M12 + left.M22 * right.M22 + left.M23 * right.M32 + left.M24 * right.M42;
        result.M23 = left.M21 * right.M13 + left.M22 * right.M23 + left.M23 * right.M33 + left.M24 * right.M43;
        result.M24 = left.M21 * right.M14 + left.M22 * right.M24 + left.M23 * right.M34 + left.M24 * right.M44;
        result.M31 = left.M31 * right.M11 + left.M32 * right.M21 + left.M33 * right.M31 + left.M34 * right.M41;
        result.M32 = left.M31 * right.M12 + left.M32 * right.M22 + left.M33 * right.M32 + left.M34 * right.M42;
        result.M33 = left.M31 * right.M13 + left.M32 * right.M23 + left.M33 * right.M33 + left.M34 * right.M43;
        result.M34 = left.M31 * right.M14 + left.M32 * right.M24 + left.M33 * right.M34 + left.M34 * right.M44;
        result.M41 = left.M41 * right.M11 + left.M42 * right.M21 + left.M43 * right.M31 + left.M44 * right.M41;
        result.M42 = left.M41 * right.M12 + left.M42 * right.M22 + left.M43 * right.M32 + left.M44 * right.M42;
        result.M43 = left.M41 * right.M13 + left.M42 * right.M23 + left.M43 * right.M33 + left.M44 * right.M43;
        result.M44 = left.M41 * right.M14 + left.M42 * right.M24 + left.M43 * right.M34 + left.M44 * right.M44;
    }

    // transforms the given Vector3d by this matrix,
    // projecting the result back into the W=1 plane.
    Vector3d Transform(Vector3d point)
    {
        return {
            point.X * M11 + point.Y * M21 + point.Z * M31 + M41,
            point.X * M12 + point.Y * M22 + point.Z * M32 + M42,
            point.X * M13 + point.Y * M23 + point.Z * M33 + M43
        };
    }
};