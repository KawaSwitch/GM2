#pragma once

#include "LookAt.h"
#include "Ortho.h"
#include "Box.h"

// ƒJƒƒ‰
class Camera
{
private:

    LookAt _lookAt;
    Ortho _ortho;

    Point3d _rotateCenter; // ‰ñ“]’†S

public:

    Camera()
    {
        
    }
};