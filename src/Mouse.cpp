#define _USE_MATH_DEFINES

#include "GV.h"
#include "Scene.h"
#include "Callback.h"
#include "Quaternion.h"
#include "Picking.h"

extern Scene* scene;

void Mouse(int button, int state, int x, int y)
{
    // �E�F��]
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
#ifdef _DEBUG
        //// �s�N�Z���̃f�v�X�l���擾
        //float z = GetDepth(x, y);

        //// �����ꏊ�̍��W�l���擾
        //auto coord = GetWorldCoord(x, y, z);

        //printf("mouse : x, y, depth : %d %d %f\n", x, y, z);
        //printf("world : x, y, z : %f %f %f\n\n", coord.X, coord.Y, coord.Z);
#endif

        // �}�E�X�{�^�����������ʒu���L��
        mouse_X = x;
        mouse_Y = y;
        rotate_flag = GL_TRUE;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        rotate_flag = GL_FALSE;

        // �ړ����Ă���Ƃ�������]�ۑ�
        if (!(x == mouse_X && y == mouse_Y))
        {
            // �p����ۑ�
            current = target;
        }
    }

    // ���F�ړ�
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        xStart = x;
        yStart = y;
        move_flag = GL_TRUE;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        move_flag = GL_FALSE;
    }

    // ����ȊO�͂ǂ���ł��Ȃ�
    else
    {
        rotate_flag = GL_FALSE;
        move_flag = GL_FALSE;
    }

    glutPostRedisplay(); // �r����return�Ŕ�����Ƃ����܂ŗ����ɍĕ`��ł��Ȃ��̂Œ���!
}

void Motion(int x, int y)
{
    if (rotate_flag == GL_FALSE && move_flag == GL_FALSE)
        return;

    if (rotate_flag)
    {
        // �ړ��ʂ��v�Z
        double dx = (x - mouse_X) * 1.33 / width;
        double dy = (y - mouse_Y) * 1.0 / height;

        // �N�H�[�^�j�I���̒���
        double length = sqrt(dx * dx + dy * dy);

        if (length != 0.0)
        {
            double radian = length * M_PI;
            double theta = sin(radian) / length;

            // ��]��̎p��
            Quaternion after = { dy * theta, dx * theta, 0.0, cos(radian) };

            target = after * current;
            CalcRotateMatrix(rot_mat, target);
        }
    }

    if (move_flag)
    {
        double xdis, ydis;
        double a = 0.5; // �����p

        xdis = x - xStart;
        ydis = y - yStart;

        if (move_flag == GL_TRUE)
        {
            dist_X += xdis * a * a * a * a * a;
            dist_Y -= ydis * a * a * a * a * a; // �E�B���h�E���W�n�ɍ��킹��
        }

        xStart = x;
        yStart = y;
    }

    glutPostRedisplay();
}

void Wheel(int wheel_num, int direction, int x, int y)
{
    // �Y�[��
    if (direction == 1)
        dist_Z += 2.0;
    else if (direction == -1)
        dist_Z -= 2.0;

    glutPostRedisplay();
}