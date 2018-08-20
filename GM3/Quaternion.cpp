#include "Quaternion.h"

// クォータニオンのの積の定義
Quaternion &operator*(Quaternion &q1, Quaternion &q2)
{
    Quaternion q0 =
        {
            q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
            q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
            q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
        };

    q1 = q0;
    return q1;
}

// クォータニオン->回転行列を算出
void CalcRotateMatrix(double *r, Quaternion q)
{
    double x2 = q.x + q.x;
    double y2 = q.y + q.y;
    double z2 = q.z + q.z;
    double xx = q.x * x2;
    double xy = q.x * y2;
    double xz = q.x * z2;
    double yy = q.y * y2;
    double yz = q.y * z2;
    double zz = q.z * z2;
    double wx = q.w * x2;
    double wy = q.w * y2;
    double wz = q.w * z2;

    //float qw, qx, qy, qz;
    //float x2, y2, z2;
    //float xy, yz, zx;
    //float wx, wy, wz;

    //qw = q.w; qx = q.x; qy = q.y; qz = q.z;

    //x2 = 2.0f * qx * qx;
    //y2 = 2.0f * qy * qy;
    //z2 = 2.0f * qz * qz;

    //xy = 2.0f * qx * qy;
    //yz = 2.0f * qy * qz;
    //zx = 2.0f * qz * qx;

    //wx = 2.0f * qw * qx;
    //wy = 2.0f * qw * qy;
    //wz = 2.0f * qw * qz;

    //double r1[16] =
    //{
    //    1.0 - y2 - z2, xy - wz, zx + wy, 0.0,
    //    xy + wz, 1.0 - z2 - x2, yz - wx, 0.0,
    //    zx - wy, yz + wx, 1.0 - x2 - y2, 0.0,
    //    0.0, 0.0, 0.0, 1.0,
    //};

    double r1[16] =
        {
            1.0 - (yy + zz),
            xy + wz,
            xz - wy,
            0.0,
            xy - wz,
            1.0 - (xx + zz),
            yz + wx,
            0.0,
            xz + wy,
            yz - wx,
            1.0 - (xx + yy),
            0.0,
            0.0,
            0.0,
            0.0,
            1.0,
        };

    //double xx = q.x * q.x * 2.0;
    //double yy = q.y * q.y * 2.0;
    //double zz = q.z * q.z * 2.0;
    //double xy = q.x * q.y * 2.0;
    //double yz = q.y * q.z * 2.0;
    //double zx = q.z * q.x * 2.0;
    //double xw = q.x * q.w * 2.0;
    //double yw = q.y * q.w * 2.0;
    //double zw = q.z * q.w * 2.0;

    //double r1[16] =
    //{
    //    1.0 - yy - zz, xy + zw, zx - yw, 0.0,
    //    xy - zw, 1.0 - zz - xx, yz + xw, 0.0,
    //    zx + yw, yz - xw, 1.0 - xx - yy, 0.0,
    //    0.0, 0.0, 0.0, 1.0,
    //};

    for (int i = 0; i < 16; i++)
        r[i] = r1[i];
}
