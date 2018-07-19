#pragma once

#include "GV.h"

// �����N���X
// ���C���ɂ����g���\��Ȃ��̂œK��
class Light
{
private:

    GLenum lightNum;

    // �K��
    GLfloat lightDiffuse[4] = { 0.7f, 0.7f, 0.7f, 1.0f }; // �g�U��
    GLfloat lightAmbient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // ����
    GLfloat lightSpecular[4] = { 0.2f, 0.2f, 0.2f, 1.0f }; // ���ʌ�
    GLfloat* lightPosition; // �ʒu

public:

    // �����I��
    void On() const
    {
        glEnable(lightNum);
    }

    // �����I�t
    void Off() const
    {
        glDisable(lightNum);
    }

    Light(GLenum light, double posX, double posY, double posZ) 
    {
        lightNum = light;

        GLfloat pos[4] = { (GLfloat)posX, (GLfloat)posY, (GLfloat)posZ, 1.0 };
        lightPosition = pos;

        // ���C�g�ݒ�
        glLightfv(lightNum, GL_POSITION, lightPosition);
        glLightfv(lightNum, GL_DIFFUSE, lightDiffuse);
        glLightfv(lightNum, GL_AMBIENT, lightAmbient);
        glLightfv(lightNum, GL_SPECULAR, lightSpecular);
    }
};