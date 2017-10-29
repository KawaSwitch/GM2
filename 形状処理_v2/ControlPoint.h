#pragma once

#include "Object.h"

class ControlPoint : public Object
{
private:

    // Ž–‘O•`‰æ
    virtual void PreDraw()
    {
        SetColor(Color::red); // ÔF
        glPointSize(10.0);

        // •`‰æ
        glBegin(GL_POINTS);
        glVertex3d(X, Y, Z);
        glEnd();
    }

public:

    double X, Y, Z;

    ControlPoint() { X = 0; Y = 0; Z = 0; }
    ControlPoint(double x, double y, double z) { X = x; Y = y; Z = z; }

    bool operator == (const ControlPoint& other)
    {
        return this->GetDistanceTo(other) < EPS::DIST;
    }

    bool operator != (const ControlPoint& other)
    {
        return !(*this == other);
    }

    double GetDistanceTo(ControlPoint other)
    {
        return sqrt(
            (X - other.X)*(X - other.X) +
            (Y - other.Y)*(Y - other.Y) +
            (Z - other.Z)*(Z - other.Z));
    }
};