#pragma once

#include "GV.h"

// 光源クラス
// メインにしか使う予定ないので適当
class Light
{
private:

    // 適当
    GLfloat lightPosition[4] = { 0, 0, 100, 1.0 }; // 光源の位置
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 }; // 拡散光
    GLfloat lightAmbient[4] = { 0.0, 0.0, 0.0, 1.0 }; // 環境光
    GLfloat lightSpecular[4] = { 1.0, 1.0, 1.0, 1.0 }; // 鏡面光

public:

    // 光源設定
    void InitLight()
    {
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

        glEnable(GL_LIGHT0);
    }

    Light() { }
};