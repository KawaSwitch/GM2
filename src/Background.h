#pragma once

#include "GV.h"
#include "Picking.h"
#include "Camera.h"
#include "CalcUtil.h"

static GLdouble color_top[4] = { 1.0, 1.0, 1.0, 1.0 };
static GLdouble color_bottom[4] = { 0.8, 1.0, 0.9, 1.0 };

template<class IBackGround> void DrawBackground(const IBackGround& grid)
{
    grid.Draw();
}

// �w�i�C���^�[�t�F�[�X
class IBackGround
{
    void virtual Draw() const = 0;
};

static bool isBackNotDrawn = true;

// �ʏ�w�i
struct BackgroundNormal : IBackGround
{
    void Draw() const override
    {
        double elev_angle = PersParam::fovy / 2; // �p(�x���@)
        double backHeightHalf, backWidthHalf; // �w�i�̑傫��(���_����̒���)

        glPushMatrix();
        {
            // ���̐ςɍ����悤�ɔw�i�̑傫���ƈʒu�����肷��
            // ���󃁃C���r���[�͓������e�̂�
            if (mainProjType == ProjectType::Perspective)
            {
                // NOTE: �A�X��ς����Ƃ��Ƀo�O����, ����SlicerWpf�ɍ��킹��
                backHeightHalf = PersParam::zNear * std::tan(ToRad(elev_angle));
                backWidthHalf = PersParam::zNear * std::tan(ToRad(elev_angle * ((GLdouble)width / height)));

                // �����̂̑O�ʂɐݒ肷��
                // NOTE: ��ʂ��ƂȂ������炷�K�v���������̂őO�ʂɂ���
                glTranslated(0, 0, -PersParam::zNear);
            }
            else
                Error::ShowAndExit("�w�i�ݒ�s��", "���e�@�������Ȓl�ł�");


            // �w�i�`��
            glBegin(GL_QUADS);

            glColor4dv(color_top);
            glVertex3d(-backWidthHalf, backHeightHalf, 0);
            glVertex3d(backWidthHalf, backHeightHalf, 0);

            glColor4dv(color_bottom);
            glVertex3d(backWidthHalf, -backHeightHalf, 0);
            glVertex3d(-backWidthHalf, -backHeightHalf, 0);

            glEnd();
        }
        glPopMatrix();
    }
};