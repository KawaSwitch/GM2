#define _USE_MATH_DEFINES

#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Background.h"
#include "Axis.h"
#include "ControlPoint.h"
#include "BsplineCurve.h"
#include "Test.h"
#include "Picking.h"
#include "Camera.h"
#include "glUtil.h"
#include "Initialize.h"

NormalAxis* axis; // ��
GeoGrid2D* grid; // �O���b�h
Scene* scene; // �V�[��
extern Scene* test_scene;
extern const int grid_length;

static Box coverBox; // �S�̂̃{�b�N�X
static Vector3d rotateCenter; // ��]���S
static bool isFirst = true;

// --- �v���g�^�C�v�錾 ---
void SetRotateCenter();
void ShowRotateCenter(bool isRotating);
void UpdateLookAtZ(const Box* const box);
// ------------------------

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // ���N�G�X�g������Ε\���������ʒu�ɖ߂�
    if (isViewInitRequested)
    {
        InitQuaternion(); // ��]�p����������
        dist_X = dist_Y = 0.0; // �ړ���������(Z����������)

        glutPostRedisplay();
        isViewInitRequested = false;
    }

    glEnable(GL_STENCIL_TEST); // �X�e���V���L����

    // 1.���`��
    {
        // ���̌^���
        glStencilFunc(GL_ALWAYS, static_cast<int>(StencilRef::Axis), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // ���p�r���[�|�[�g�T�C�Y���v�Z
        int axisViewWidth = width / 5;
        int axisViewHeight = height / 5;
        int axisViewSize = (axisViewWidth < axisViewHeight) ? axisViewWidth : axisViewHeight;

        // ���p�r���[�|�[�g��ݒ�
        double axisLength = axis->GetLength();
        glViewport(axisViewSize / 10, axisViewSize / 10, axisViewSize, axisViewSize);

        // ���p�ɕϊ��s����w�肷��
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(
            -axisLength, axisLength,
            -axisLength, axisLength,
            -axisLength, axisLength);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // ���p���C�g�̂݃I���ɂ���
        for (const auto& light : modelLight)
            light->Off();
        for (const auto& light : axisLight)
            light->On();

        glPushMatrix();

        glMultMatrixd(rot_mat); // ��]�ʂ̓��f���Ɠ�����

        if (axisShowType != AxisShowType::None)
            axis->Draw();

        glPopMatrix();
    }


    // �r���[�|�[�g���E�B���h�E�S�̂ɐݒ�
    glViewport(0, 0, width, height);

    // �ϊ��s���S�̗p�Ɏw�肵�Ȃ���
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(PersParam::fovy, (GLdouble)width / height, PersParam::zNear, PersParam::zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 0.0, 0.0,  // ���_�ʒu
        0.0, 0.0, -1.0,  // �����ʒu
        0.0, 1.0, 0.0   // ����� : y
    );

    // �`��p���C�g�̂݃I���ɂ���
    for (const auto& light : axisLight)
        light->Off();
    for (const auto& light : modelLight)
        light->On();


    // 2. �`��`��
    {
        // �`��̌^���
        // ���������ĕ`��
        glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glPushMatrix();
        {
            // �N���ŏ��̕`��ŉ�]���S���E�B���h�E���S�ɂ���
            if (isFirst)
            {
                TestRegister(); // ���O�ɓo�^���Ă��� 

                SetRotateCenter();
                UpdateLookAtZ(&coverBox); // �t�B�b�g
            }

            glTranslated(-rotateCenter.X, -rotateCenter.Y, -rotateCenter.Z);

            glTranslated(dist_X, dist_Y, dist_Z); // �ړ�

            // ��]���S���w�肵�ĉ�]
            RotateAt(rot_mat, rotateCenter);

            // �e�X�g�`��
            TestDraw();
            // �`��`��
            scene->Draw();

            // ��]���S�`��
            ShowRotateCenter(rotate_flag);

            // �O���b�h�`��
            {
                // �O���b�h�̌^���
                // ��̑S�����`�悳��Ă��Ȃ����ɃO���b�h��`��
                glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Grid), 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

                // �􉽃O���b�h�`��
                if (gridType != GridType::InVisible)
                    grid->Draw();
            }
        }
        glPopMatrix();
    }


    // 3. �w�i�`��
    {
        // ��̑S�����`�悳��Ă��Ȃ��ӏ��͔w�i��`��
        glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::Background), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        DrawBackground(BackgroundNormal());
    }


    glDisable(GL_STENCIL_TEST); // �X�e���V��������

    isFirst = false;
    glutSwapBuffers();
}

// ��]���S��ݒ肵�܂�
void SetRotateCenter()
{
    vector<Box> sceneBoxes;

    // ��]���S�����߂�
    // ���ׂĂ̌`��̒��S����]���S�Ƃ���
    // TODO: ������ʒ��S�ŉ�]��������
    sceneBoxes.push_back(scene->GetCoverBound());
    sceneBoxes.push_back(test_scene->GetCoverBound());
    coverBox = Box(sceneBoxes);

    // �Ƃ肠����
    if (coverBox.IsValid() == false)
    {
        Error::ShowMessage(
            "�o�E���h�{�b�N�X�ݒ�G���[",
            "�`��`�󂪐ݒ肳��Ă��܂���. ����̉�]�{�b�N�X, ��]���S��ݒ肵�܂�");
        coverBox = Box(
            -grid_length, -grid_length, -grid_length,
            grid_length, grid_length, grid_length);
    }

    auto center = coverBox.Center();

    rotateCenter = Vector3d(center.X, center.Y, center.Z);
}
// ��]���S��\�����܂�
void ShowRotateCenter(bool isRotating)
{
    if (rotate_flag)
    {
        // ���C�e�B���O�͐؂��Ă���
        if (glIsEnabled(GL_LIGHTING))
            glDisable(GL_LIGHTING);

        // ���S�_�`�� �f�v�X�l�͕]�����Ȃ�
        glColor4dv(Color::orange);
        glPointSize(10.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::orange);

        glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::RotateCenter), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glDisable(GL_DEPTH_TEST);
        glBegin(GL_POINTS);
        glVertex3d(rotateCenter);
        glEnd();
        glEnable(GL_DEPTH_TEST);

        // �J�o�[�{�b�N�X�`��  �`��̌��ɂ���Γ_���\��
        glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        coverBox.Draw(Color::orange, 2.0); // ���ʂɕ`��

        // �A���͔j���\��
        {
            glStencilFunc(GL_GREATER, static_cast<int>(StencilRef::HiddenLine), 0xFF);
            glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
            coverBox.Draw(Color::orange, 2.0); // �A������p

            glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::HiddenLine), 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0xF0F0);

            // �f�v�X�l�͕]�����Ȃ�
            glDisable(GL_DEPTH_TEST);
            coverBox.Draw(Color::orange, 1.5);
            glEnable(GL_DEPTH_TEST);

            glDisable(GL_LINE_STIPPLE);
        }

        glPointSize(1.0);
    }
}

// ���_��Z�ʒu���{�b�N�X���猈�肵�X�V���܂�
void UpdateLookAtZ(const Box* const box)
{
    double boxHalfX = (box->MaxX() - box->MinX()) / 2;
    double boxHalfY = (box->MaxY() - box->MinY()) / 2;
    double boxHalfZ = (box->MaxZ() - box->MinZ()) / 2;
    double boxLongerHalfEdge = (boxHalfX > boxHalfY) ? boxHalfX : boxHalfX;
    double boxAspectForFit = (boxHalfX > boxHalfY) ? (boxHalfX / boxHalfY) : (boxHalfY / boxHalfX);

    double ratio; // �p�ɑ΂���c����{��

    // �E�B���h�E�ɂ҂�������܂�悤�ȏc����{�����ꍇ����
    if (width > height)
        ratio = boxHalfX / boxHalfY;
    else
        ratio = (double)width / height;

    // �{�b�N�X�O�ʂ���J�����܂ł̋���
    double distToBoxFront = boxHalfX / std::tan(ratio * (PersParam::fovy / 2) * (M_PI / 180));

    // �J�����͌��_�ɒu�����܂܃��[���h�𔽑Ε����ɓ�����
    double cameraZ = boxHalfZ + distToBoxFront;
    dist_Z = -(cameraZ + boxHalfZ / 2); // ������Ɨ]�T���������Ă���
}

// �R���\�[���ɐ�����\�����܂�
void ShowConsoleDiscription()
{
    puts("�`�󏈗�C++ ������@\n");

    puts("$ �t�@�C���N�����ǂݍ���");
    puts("./KJS_FILE���Ŗ��O�̐擪��@������.kjs�t�@�C�����N�����ɓǂݍ��݂܂�\n");

    puts("$ �}�E�X����");
    ShowButtonDiscription("�E�h���b�O", "��]");
    ShowButtonDiscription("���h���b�O", "�ړ�");
    ShowButtonDiscription("�z�C�[��", "�g��/�k��");

    puts("");

    // �G���e�B�e�B����{�^������
    puts("$ �G���e�B�e�B�g�O���\���n");
    ShowButtonDiscription("B", "�~�j�}�N�X�{�b�N�X");
    ShowButtonDiscription("P", "����_");
    ShowButtonDiscription("F", "��K����(�ڐ�)�x�N�g��");
    ShowButtonDiscription("S", "��K�����x�N�g��");
    ShowButtonDiscription("C", "�ȗ��x�N�g��");
    ShowButtonDiscription("N", "�@���x�N�g��");

    puts("");

    puts("$ ���̑��\���n");
    ShowButtonDiscription("ESC", "�I��");
    ShowButtonDiscription("A", "���\���ύX");
    ShowButtonDiscription("G", "�O���b�h�\���ύX");
    ShowButtonDiscription("I", "��]/�ړ�(���s�ȊO)�����ɖ߂�");
    //ShowButtonDiscription("L", "���C�e�B���O�g�O��");

    puts("\n");

    puts("�f�o�O�p�o�́�\n");
}

// �{�^���̐�����\�����܂�
void ShowButtonDiscription(char* button, char* disc)
{
    printf("    ");
    printf("%s : ", button);
    printf("%s\n", disc);
}