#include "GV.h"
#include "CalcUtil.h"
#include "glUtil.h"

// �_�Q��`�悷��
void DrawPoints(vector<Vector3d> vec, GLdouble* color, GLdouble pSize)
{
    glColor4dv(color);
    glPointSize(pSize);

    glBegin(GL_POINTS);
    {
        for (auto it = vec.begin(); it != vec.end(); ++it)
            glVertex3d((*it));
    }
    glEnd();

    glPointSize(1.0); // �߂��Ă���
}

// ���֐���`�悷��
void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t, double max_t)
{
    // �f�B�X�v���C���X�g
    static int displayList = 0;

    // �덷�h�~
    int MIN = (int)(min_t * 100);
    int MAX = (int)(max_t * 100);

    glLineWidth(1.5);

    if (displayList != 0)
    {
        // �f�B�X�v���C���X�g�쐬�ς݂Ȃ�R�[��
        glCallList(displayList);
    }
    else
    {
        if (!(displayList = glGenLists(1)))
            Error::ShowAndExit("�f�B�X�v���C���X�g�쐬���s");

        glNewList(displayList, GL_COMPILE);

        // ���֐��͐���_������
        for (int i = 0; i < ncpnt; i++)
        {
            glBegin(GL_LINE_STRIP);

            // �F�̓����_��
            GLdouble color[4];
            Color::GetRandomColor(color);
            glColor4dv(color);

            for (int T = MIN; T <= MAX; T++)
            {
                double t = (double)T / 100.0;

                double bf = CalcBsplineFunc(i, mord, t, knot);
                glVertex3d(t, bf, 0);
            }

            glEnd();
        }

        glEndList();
    }
}

// �f�B�X�v���C���X�g�ƕ`��֐���p���ĕ`�悷��
void DrawUsingDisplayList(int* const displayList, const function<void(void)> RegisterDraw)
{
    if (*displayList == 0)
    {
        if (!(*displayList = glGenLists(1)))
            return;

        // �o�^
        glNewList(*displayList, GL_COMPILE);
        RegisterDraw();
        glEndList();

        glutPostRedisplay();
    }
    else
        glCallList(*displayList);
}