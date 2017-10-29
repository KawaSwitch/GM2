#pragma once

struct Vector3d
{
    double X, Y, Z;

    Vector3d() { X = 0; Y = 0; Z = 0; }
    Vector3d(double x, double y, double z) { X = x; Y = y; Z = z; }
};