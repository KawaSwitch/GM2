#include "GV.h"
#include "Scene.h"
#include "Picking.h"

extern Scene* scene;

// �������ɸ�Υǥץ��ͤ��������
float GetDepth(int x, int y)
{
    float z;
    GLint viewport[4];  // �ӥ塼�ݡ���
                        
    // �ǥХ�����ɸ�Ϥȥ�����ɥ���ɸ�Ϥ��Ѵ�
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadBuffer(GL_BACK);

    glReadPixels(x, viewport[3] - y - 1, 1, 1,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        &z);

    return z;
}

// �������ɸ������ɺ�ɸ���������
Vector3d GetWorldCoord(int x, int y, float depth)
{
    double wx, wy, wz;
    GLdouble mvMatrix[16], pjMatrix[16];
    GLint viewport[4];

    // �ѥ�᡼������
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);

    // ������ɸ���������
    gluUnProject((double)x, (double)viewport[3] - y - 1, depth,
        mvMatrix,
        pjMatrix,
        viewport,
        &wx,
        &wy,
        &wz);

    return Vector3d(wx, wy, wz);
}

// ���ɺ�ɸ����������ɸ���������
Vector3d GetLocalCoord(int x, int y, int z)
{
    double winX, winY, depth;
    GLdouble mvMatrix[16], pjMatrix[16];
    GLint viewport[4];

    // �ѥ�᡼������
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);

    // ������ɸ���������
    gluProject((double)x, (double)viewport[3] - y - 1, z,
        mvMatrix,
        pjMatrix,
        viewport,
        &winX,
        &winY,
        &depth);

    return Vector3d(winX, winY, depth);
}

// �ޥ����κ�ɸ�ȽŤʤäƤ��륪�֥������Ȥμ����ֹ���֤�
unsigned int GetObjNumberOnMousePointer(int x, int y)
{
    GLuint selectBuf[128] = { 0 };  // ���쥯�����Хåե�
    GLuint hits; // �ҥåȥʥ�С�
    GLint viewport[4];  // �ӥ塼�ݡ���
    unsigned int objNum = 0;

    // ���쥯����󳫻�

    // ���ߤΥӥ塼�ݡ��Ȥ����
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // ���쥯������ѥХåե���OpenGL���Ϥ�
    glSelectBuffer(BUF_MAX, selectBuf);
    
    // OpenGL����⡼�ɤ򥻥쥯�����⡼�ɤ�
    (void)glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    // ����Ѵ����������оݤȤ������ߤ�����Ѵ�����򥹥��å��������
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    // ��ƹ��������
    glLoadIdentity();

    // �ޥ����ԥå��󥰤��ϰϤ�����
    gluPickMatrix(x,
        viewport[3] - y, // ������ɥ����������Ȥ���������ɥ�y��ɸ��Ϳ����
        100.0, // �ԥ�����ñ�̤Υ��쥯������ϰ� X��Y
        100.0, // �����ߤ�Ĵ��
        viewport);

    glMatrixMode(GL_MODELVIEW);
    // ������glLoadName�Ĥ������褷��̾����Ĥ���
    // ���ؤο�����1�ˤ��뤳�ȡ�
    scene->DrawForPick();


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // �������⡼�ɤ��᤹
    hits = glRenderMode(GL_RENDER); // �ҥåȥ쥳���ɤ������

    // ���ּ����Υ��֥������Ȥ��֤�
    if (hits > 0)
        objNum = GetNearestNumber(hits, selectBuf);

    glMatrixMode(GL_MODELVIEW);        //��ǥ�ӥ塼�⡼�ɤ��᤹

    // ���Ĥ���ʤ����0���֤�
    return objNum;
}

// ����1�Ȥ�������ǡ�����������free��˺��ʤ���, ��ǥ��ޡ��ȥݥ��󥿼�����
void GetSelectionData(GLuint hits, GLuint* buf, SelectionData* datas)
{
    // ����1�Ȥ��ƥǡ���������
    for (unsigned int i = 0, dataI = 0; i < 128; dataI++)
    {
        if (buf[i++] == 0)
            break;

        datas[dataI].minZ = buf[i++];
        datas[dataI].maxZ = buf[i++];
        datas[dataI].name = buf[i++];
    }
}

// ��äȤ�����ˤ��륪�֥��������ֹ�����
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

// ��äȤ���ˤ��륪�֥��������ֹ�����
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
