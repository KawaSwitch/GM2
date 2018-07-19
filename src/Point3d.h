#pragma once

// 3�������W��1�_
class Point3d
{
public:
    
    double X, Y, Z;

    // Vector3d�݊�
    explicit operator Vector3d() { return Vector3d(X, Y, Z); }

    Point3d(double x, double y, double z) { X = x; Y = y; Z = z; }
    Point3d(const Vector3d& v) { X = v.X; Y = v.Y; Z = v.Z; }
};

// �Ȑ���̍��W
class Point3dC : public Point3d
{
public:

    double param; // �p�����[�^

    Point3dC(double x, double y, double z, double t) : Point3d(x, y, z) { param = t; }
    Point3dC(const Vector3d& v, double t) : Point3d(v) { param = t; }
};

// �Ȗʏ�̍��W
class Point3dS : public Point3d
{
public:

    double paramU, paramV; // �p�����[�^

    Point3dS(double x, double y, double z, double u, double v) : Point3d(x, y, z) { paramU = u;  paramV = v; }
    Point3dS(const Vector3d& vec, double u, double v) : Point3d(vec) { paramU = u; paramV = v; }
};