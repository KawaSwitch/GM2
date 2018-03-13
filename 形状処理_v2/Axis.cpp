#include "Axis.h"
#include "Color.h"

void NormalAxis::Draw()
{
    // ディスプレイリスト
    static int displayList;
    static bool isRendered = false;

    if (isUseLight)
        glEnable(GL_LIGHTING);

    if (isRendered)
    {
        // ディスプレイリスト作成済みならコール
        glCallList(displayList);
    }
    else
    {
        if (!(displayList = glGenLists(1)))
            Error::ShowAndExit("ディスプレイリスト作成失敗");

        glNewList(displayList, GL_COMPILE);

        // for position
        //glDisable(GL_DEPTH_TEST);
        //this->DrawInternal();

        // actual drawing
        glEnable(GL_DEPTH_TEST);
        this->DrawInternal();

        glEndList();
        isRendered = true;
    }

    if (isUseLight)
        glDisable(GL_LIGHTING);
}

void NormalAxis::DrawInternal()
{
    GLUquadricObj* base_arrows[3];
    GLUquadricObj* arrows[3];
    GLdouble specular[4] = { 0.125, 0.125, 0.125, 1.0 };

    // X - axis
    if (isUseLight)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::red);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    }
    else
        glColor4dv(Color::red);

    glPushMatrix();
    base_arrows[0] = gluNewQuadric();
    gluQuadricDrawStyle(base_arrows[0], GLU_FILL);
    glRotated(90.0f, 0, 1, 0);
    gluCylinder(base_arrows[0], _length / 20, _length / 20, _length, 25, 25);
    glPopMatrix();

    glPushMatrix();
    arrows[0] = gluNewQuadric();
    gluQuadricDrawStyle(arrows[0], GLU_FILL);
    glTranslated(_length, 0.0f, 0.0f);
    glRotated(90.0f, 0, 1, 0);
    gluCylinder(arrows[0], _length / 10, 0.0f, _length / 4, 25, 25);
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
    base_arrows[0] = gluNewQuadric();
    gluQuadricDrawStyle(base_arrows[0], GLU_FILL);
    glRotated(-90.0f, 1, 0, 0);
    gluCylinder(base_arrows[0], _length / 20, _length / 20, _length, 25, 25);
    glPopMatrix();

    glPushMatrix();
    arrows[1] = gluNewQuadric();
    gluQuadricDrawStyle(arrows[1], GLU_FILL);
    glTranslated(0.0f, _length, 0.0f);
    glRotated(-90.0f, 1, 0, 0);
    gluCylinder(arrows[1], _length / 10, 0.0f, _length / 4, 25, 25);
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
    base_arrows[0] = gluNewQuadric();
    gluQuadricDrawStyle(base_arrows[0], GLU_FILL);
    gluCylinder(base_arrows[0], _length / 20, _length / 20, _length, 25, 25);
    glPopMatrix();

    glPushMatrix();
    arrows[2] = gluNewQuadric();
    gluQuadricDrawStyle(arrows[2], GLU_FILL);
    glTranslated(0.0f, 0.0f, _length);
    gluCylinder(arrows[2], _length / 10, 0.0f, _length / 4, 25, 25);
    glPopMatrix();

    glLineWidth(1.0);
}