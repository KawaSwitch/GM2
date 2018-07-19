#include "GV.h"
#include "Scene.h"
#include "Picking.h"

extern Scene* scene;

// ���[�J�����W�̃f�v�X�l���擾����
float GetDepth(int x, int y)
{
    float z;
    GLint viewport[4];  // �r���[�|�[�g
                        
    // �f�o�C�X���W�n�ƃE�B���h�E���W�n�̕ϊ�
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadBuffer(GL_BACK);

    glReadPixels(x, viewport[3] - y - 1, 1, 1,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        &z);

    return z;
}

// ���[�J�����W���烏�[���h���W���擾����
Vector3d GetWorldCoord(int x, int y, float depth)
{
    double wx, wy, wz;
    GLdouble mvMatrix[16], pjMatrix[16];
    GLint viewport[4];

    // �p�����[�^�擾
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);

    // ���E���W���擾����
    gluUnProject((double)x, (double)viewport[3] - y - 1, depth,
        mvMatrix,
        pjMatrix,
        viewport,
        &wx,
        &wy,
        &wz);

    return Vector3d(wx, wy, wz);
}

// ���[���h���W���烍�[�J�����W���擾����
Vector3d GetLocalCoord(int x, int y, int z)
{
    double winX, winY, depth;
    GLdouble mvMatrix[16], pjMatrix[16];
    GLint viewport[4];

    // �p�����[�^�擾
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);

    // ���E���W���擾����
    gluProject((double)x, (double)viewport[3] - y - 1, z,
        mvMatrix,
        pjMatrix,
        viewport,
        &winX,
        &winY,
        &depth);

    return Vector3d(winX, winY, depth);
}

// �}�E�X�̍��W�Əd�Ȃ��Ă���I�u�W�F�N�g�̎��ʔԍ���Ԃ�
unsigned int GetObjNumberOnMousePointer(int x, int y)
{
    GLuint selectBuf[128] = { 0 };  // �Z���N�V�����o�b�t�@
    GLuint hits; // �q�b�g�i���o�[
    GLint viewport[4];  // �r���[�|�[�g
    unsigned int objNum = 0;

    // �Z���N�V�����J�n

    // ���݂̃r���[�|�[�g���擾
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // �Z���N�V�����p�o�b�t�@��OpenGL�֓n��
    glSelectBuffer(BUF_MAX, selectBuf);
    
    // OpenGL�`�惂�[�h���Z���N�V�������[�h��
    (void)glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    // ���e�ϊ��s��𑀍�ΏۂƂ��A���݂̓��e�ϊ��s����X�^�b�N�֓����
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    // ���e�s���������
    glLoadIdentity();

    // �}�E�X�s�b�L���O�͈̔͂̐ݒ�
    gluPickMatrix(x,
        viewport[3] - y, // �E�B���h�E���������_�Ƃ����E�B���h�Ey���W��^����
        100.0, // �s�N�Z���P�ʂ̃Z���N�V�����͈� X��Y
        100.0, // ���D�݂Œ���
        viewport);

    glMatrixMode(GL_MODELVIEW);
    // ������glLoadName���ŕ`�悵�Ė��O������
    // �K�w�̐[����1�ɂ��邱�ƁI
    scene->DrawForPick();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // �����_�[���[�h�֖߂�
    hits = glRenderMode(GL_RENDER); // �q�b�g���R�[�h�̖߂�l

    // ��Ԏ�O�̃I�u�W�F�N�g��Ԃ�
    if (hits > 0)
        objNum = GetNearestNumber(hits, selectBuf);

    glMatrixMode(GL_MODELVIEW);        //���f���r���[���[�h�֖߂�

    // ������Ȃ����0��Ԃ�
    return objNum;
}

// �K�w1�Ƃ��đI���f�[�^�𐮗��ifree���Y��Ȃ���, ��ŃX�}�[�g�|�C���^�����j
void GetSelectionData(GLuint hits, GLuint* buf, SelectionData* datas)
{
    // �K�w1�Ƃ��ăf�[�^�𐮗�
    for (unsigned int i = 0, dataI = 0; i < 128; dataI++)
    {
        if (buf[i++] == 0)
            break;

        datas[dataI].minZ = buf[i++];
        datas[dataI].maxZ = buf[i++];
        datas[dataI].name = buf[i++];
    }
}

// �����Ƃ���O�ɂ���I�u�W�F�N�g�ԍ����擾
unsigned int GetNearestNumber(GLuint hits, GLuint* buf)
{
    SelectionData* data = (SelectionData *)malloc(sizeof(SelectionData) * hits);
    GetSelectionData(hits, buf, data);

    if (data == NULL)
        return 0;

    SelectionData nearest = data[0];

    for (unsigned int i = 1; i < hits; i++)
    {
        if (data[i].minZ < nearest.minZ)
            nearest = data[i];
    }

    auto nearest_num = nearest.name;

    free(data);
    return nearest_num;
}

// �����Ƃ����ɂ���I�u�W�F�N�g�ԍ����擾
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf)
{
    SelectionData* data = (SelectionData *)malloc(sizeof(SelectionData) * hits);
    GetSelectionData(hits, buf, data);

    if (data == NULL)
        return 0;

    SelectionData farthest = data[0];

    for (unsigned int i = 1; i < hits; i++)
    {
        if (data[i].maxZ > farthest.maxZ)
            farthest = data[i];
    }

    free(data);
    return farthest.name;
}