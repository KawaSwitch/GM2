#pragma once

#include "Point3d.h"
#include "Vector.h"

class LookAt
{
public:

    Point3d _eye;
    Point3d _center;
    Vector3d _up;

    LookAt()
    {
        _eye = Point3d(0, 0, 9);
        _center = Point3d(0, 0, 0);
        _up = Vector3d(0, 1, 0);
    }
};