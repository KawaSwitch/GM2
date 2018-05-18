#pragma once

// 3次元座標の1点
class Point3d
{
public:
    
    double X, Y, Z;

    // Vector3d互換
    Vector3d ToVector() { return Vector3d(X, Y, Z); }

    Point3d(double x, double y, double z) { X = x; Y = y; Z = z; }
    Point3d(const Vector3d& v) { X = v.X; Y = v.Y; Z = v.Z; }
};

// 曲線上の座標
class Point3dC : public Point3d
{
public:

    double param; // パラメータ

    Point3dC(double x, double y, double z, double t) : Point3d(x, y, z) { param = t; }
    Point3dC(const Vector3d& v, double t) : Point3d(v) { param = t; }
};

// 曲面上の座標
class Point3dS : public Point3d
{
public:

    double paramU, paramV; // パラメータ

    Point3dS(double x, double y, double z, double u, double v) : Point3d(x, y, z) { paramU = u;  paramV = v; }
    Point3dS(const Vector3d& vec, double u, double v) : Point3d(vec) { paramU = u; paramV = v; }
};