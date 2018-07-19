#include "GV.h"

// OpenGL�Ɋւ��郆�[�e�B���e�B���`���܂�
// �`��n��DrawUtil.cpp�Œ�`���Ă�������

void glVertex3d(const Vector3d& vec) { glVertex3d(vec.X, vec.Y, vec.Z); }
void glPointSize(const GLdouble size) { glPointSize((GLfloat)size); }
void glLineWidth(const GLdouble width) { glLineWidth((GLfloat)width); }
void glNormal3d(const Vector3d& vec) { glNormal3d(vec.X, vec.Y, vec.Z); }

void glColor(const double r, const double g, const double b)
{
    const double max_8bit = 255;
    glColor3d(r / max_8bit, g / max_8bit, b / max_8bit);
}
void glColor(const double r, const double g, const double b, const double a)
{
    const double max_8bit = 255;
    glColor4d(r / max_8bit, g / max_8bit, b / max_8bit, a / max_8bit);
}

void glMaterialfv(GLenum face, GLenum pname, const GLdouble params[4])
{
    GLfloat color[4] = { (GLfloat)params[0], (GLfloat)params[1], (GLfloat)params[2], (GLfloat)params[3] };
    glMaterialfv(face, pname, color);
}

void glVertex(const vector<Vector3d> vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
        glVertex3d(*it);
}

// ��]���S���w�肵�ĉ�]
void RotateAt(const double* mat, const Vector3d center)
{
    // ���_�ɖ߂��ĉ�]����(�s��̊|���Z�͋t!)
    glTranslated(center.X, center.Y, center.Z);
    glMultMatrixd(mat);
    glTranslated(-center.X, -center.Y, -center.Z);
}

// ���݂̃r���[�|�[�g���擾���܂�
void GetCurrentViewPort(std::unique_ptr<int>& viewPort)
{
    int* array = new int[4];

    glGetIntegerv(GL_VIEWPORT, array);
    viewPort = make_unique<int>(*array);
}
// ���݂̃v���W�F�N�V�����s����擾���܂�
void GetCurrentProjectionMatrix(std::unique_ptr<double>& projection)
{
    double* array = new double[16];

    glGetDoublev(GL_PROJECTION_MATRIX, array);
    projection = make_unique<double>(*array);
}
// ���݂̃��f���r���[�s����擾���܂�
void GetCurrentModelviewMatrix(std::unique_ptr<double>& modelview)
{
    double* array = new double[16];

    glGetDoublev(GL_MODELVIEW_MATRIX, array);
    modelview = make_unique<double>(*array);
}

// ���[���h���W�����[�J�����W�֕ϊ�����
vector<Vector3d> GetLocalCoord(int size, const Vector3d* const pnt)
{
    std::vector<Vector3d> local;

    std::unique_ptr<int> viewport;
    std::unique_ptr<double> projection, modelview;

    GetCurrentViewPort(viewport);
    GetCurrentProjectionMatrix(projection);
    GetCurrentModelviewMatrix(modelview);

    //auto a = viewport.get();
    //for (int i = 0; i < 4; i++)
    //    cout << *(a++) << " ";

    //cout << endl;

    //for (int i = 0; i < size; ++i)
    //{
    //    double winX, winY, winZ;
    //    gluProject(
    //        pnt[i].X, pnt[i].Y, pnt[i].Z,
    //        modelview.get(), projection.get(), viewport.get(), 
    //        &winX, &winY, &winZ);

    //    local.push_back(Vector3d(winX, winY, winZ));
    //}

    int v[4];
    double p[16], m[16];
    glGetIntegerv(GL_VIEWPORT, v);
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetDoublev(GL_MODELVIEW_MATRIX, m);

    for (int i = 0; i < size; ++i)
    {
        double winX, winY, winZ;
        gluProject(
            pnt[i].X, pnt[i].Y, pnt[i].Z,
            m, p, v,
            &winX, &winY, &winZ);

        local.push_back(Vector3d(winX, winY, winZ));
    }

    return local;
}

