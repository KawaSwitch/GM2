#include "Axis.h"
#include "Color.h"
#include "glUtil.h"

AxisShowType axisShowType = AxisShowType::None;

void NormalAxis::Draw()
{
    // �f�B�X�v���C���X�g
    static int displayList;
    static int displayListLabel; // �����p
    static bool isRendered = false;

    if (isUseLight)
        glEnable(GL_LIGHTING);

    if (isRendered)
    {
        // �f�B�X�v���C���X�g�쐬�ς݂Ȃ�R�[��
        if (axisShowType != AxisShowType::None)
        {
            glCallList(displayList);

            // ���x���\��
            if (axisShowType == AxisShowType::WithLabel)
                glCallList(displayListLabel);
        }
    }
    else // ����͓o�^
    {
        // ��
        {
            if (!(displayList = glGenLists(1)))
                Error::ShowAndExit("�f�B�X�v���C���X�g�쐬���s");

            glNewList(displayList, GL_COMPILE);

            // actual drawing
            glEnable(GL_DEPTH_TEST);
            this->DrawInternal();

            glEndList();
        }

        // ����
        {
            if (!(displayListLabel = glGenLists(1)))
                Error::ShowAndExit("�f�B�X�v���C���X�g�쐬���s");

            glNewList(displayListLabel, GL_COMPILE);

            glEnable(GL_DEPTH_TEST);
            this->DrawCharacter();

            glEndList();
        }

        isRendered = true;
    }

    if (isUseLight)
        glDisable(GL_LIGHTING);
}

void NormalAxis::DrawInternal()
{
    GLUquadricObj* base_arrows[3];
    GLUquadricObj* arrows[3];
    GLdouble specular[4] = { 0.15, 0.15, 0.15, 1.0 };

    // X - axis
    if (isUseLight)
    {
        glEnable(GL_LIGHTING);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::red);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glColor4dv(Color::red);
    }

    // ���̓������`��
    glPushMatrix();
    {
        base_arrows[0] = gluNewQuadric();
        gluQuadricDrawStyle(base_arrows[0], GLU_FILL);
        glRotated(90.0f, 0, 1, 0);
        gluCylinder(base_arrows[0], _length / 20, _length / 20, _bodyLength, 25, 25);
    }
    glPopMatrix();

    glPushMatrix();
    {
        // ���̖�󕔕��`��
        arrows[0] = gluNewQuadric();
        gluQuadricDrawStyle(arrows[0], GLU_FILL);
        glTranslated(_bodyLength, 0.0f, 0.0f);
        glRotated(90.0f, 0, 1, 0);
        gluCylinder(arrows[0], _length / 10, 0.0f, _arrowLength, 25, 25);
    }
    glPopMatrix();

    // Y - axis
    if (isUseLight)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::green);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    }
    else
        glColor4dv(Color::green);

    glPushMatrix();
    {
        base_arrows[0] = gluNewQuadric();
        gluQuadricDrawStyle(base_arrows[0], GLU_FILL);
        glRotated(-90.0f, 1, 0, 0);
        gluCylinder(base_arrows[0], _length / 20, _length / 20, _bodyLength, 25, 25);
    }
    glPopMatrix();

    glPushMatrix();
    {
        arrows[1] = gluNewQuadric();
        gluQuadricDrawStyle(arrows[1], GLU_FILL);
        glTranslated(0.0f, _bodyLength, 0.0f);
        glRotated(-90.0f, 1, 0, 0);
        gluCylinder(arrows[1], _length / 10, 0.0f, _arrowLength, 25, 25);
    }
    glPopMatrix();

    // Z - axis
    if (isUseLight)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::blue);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    }
    else
        glColor4dv(Color::blue);

    glPushMatrix();
    {
        base_arrows[0] = gluNewQuadric();
        gluQuadricDrawStyle(base_arrows[0], GLU_FILL);
        gluCylinder(base_arrows[0], _length / 20, _length / 20, _bodyLength, 25, 25);
    }
    glPopMatrix();

    glPushMatrix();
    {
        arrows[2] = gluNewQuadric();
        gluQuadricDrawStyle(arrows[2], GLU_FILL);
        glTranslated(0.0f, 0.0f, _bodyLength);
        gluCylinder(arrows[2], _length / 10, 0.0f, _arrowLength, 25, 25);
    }
    glPopMatrix();

    // Origin
    if (isUseLight)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::orange);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    }
    else
        glColor4dv(Color::orange);

    glutSolidSphere(_length / 20, 30, 30);

    glLineWidth(1.0);
}

void NormalAxis::DrawCharacter()
{
    glDisable(GL_LIGHTING);
    glColor(204, 40, 255); // ���F

    glDepthFunc(GL_ALWAYS); // �ʓ|�Ȃ̂őS���ʂ�

    // X - axis
    glPushMatrix();
    {
        glTranslated(_length, 0.0f, 0.0f);
        glRotated(90.0f, 0, 1, 0);

        glRasterPos3f(0, 0, -1.0f);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    }
    glPopMatrix();

    // Y - axis
    glPushMatrix();
    {
        glTranslated(0.0f, _length, 0.0f);
        glRotated(-90.0f, 1, 0, 0);

        glRasterPos3f(0, 0, -1.0f);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');
    }
    glPopMatrix();

    // Z - axis
    glPushMatrix();
    {
        glTranslated(0.0f, 0.0f, _length);

        glRasterPos3f(0, 0, -1.0f);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');
    }
    glPopMatrix();

    glDepthFunc(GL_LESS);
}