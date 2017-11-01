#pragma once

#include "GV.h"
#include "Object.h"

// デバグクラス
// 三角形状
class Triangle : public Object
{
private:

    Point3d A, B, C;

    // 事前描画
    virtual void PreDraw() override
    {
        glColor3d(1.0, 0.0, 0.0); // とりあえず赤
        glBegin(GL_TRIANGLES);
        glVertex3d(A.X, A.Y, A.Z);
        glVertex3d(B.X, B.Y, B.Z);
        glVertex3d(C.X, C.Y, C.Z);
        glEnd();
    }

public:

    Triangle(Point3d a, Point3d b, Point3d c) { A = a; B = b; C = c; }
    ~Triangle() { }
};