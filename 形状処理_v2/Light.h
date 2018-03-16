#pragma once

#include "GV.h"

// 光源クラス
// メインにしか使う予定ないので適当
class Light
{
private:

    GLenum lightNum;

    // 適当
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 }; // 拡散光
    GLfloat lightAmbient[4] = { 1.0, 1.0, 1.0, 1.0 }; // 環境光
    GLfloat lightSpecular[4] = { 0.1f, 0.1f, 0.1f, 1.0 }; // 鏡面光
    GLfloat* lightPosition; // 位置

public:

    // 光源オン
    void On()
    {
        glEnable(lightNum);
    }

    // 光源オフ
    void Off()
    {
        glDisable(lightNum);
    }

    Light(GLenum light, double posX, double posY, double posZ) 
    {
        lightNum = light;

        GLfloat pos[4] = { (GLfloat)posX, (GLfloat)posY, (GLfloat)posZ, 1.0 };
        lightPosition = pos;

        glLightfv(lightNum, GL_POSITION, lightPosition);
        glLightfv(lightNum, GL_DIFFUSE, lightDiffuse);
        glLightfv(lightNum, GL_AMBIENT, lightAmbient);
        glLightfv(lightNum, GL_SPECULAR, lightSpecular);
    }
};