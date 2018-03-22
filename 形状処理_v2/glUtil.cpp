#include "GV.h"

// OpenGLに関するユーティリティを定義します
// 描画系はDrawUtil.cppで定義してください

void glVertex3d(const Vector3d& vec) { glVertex3d(vec.X, vec.Y, vec.Z); }
void glVertex3d(const Point3d& pnt) { glVertex3d(pnt.X, pnt.Y, pnt.Z); }
void glPointSize(const GLdouble size) { glPointSize((GLfloat)size); }
void glLineWidth(const GLdouble width) { glLineWidth((GLfloat)width); }
void glNormal3d(const Vector3d& vec) { glNormal3d(vec.X, vec.Y, vec.Z); }

void glMultMatrixd(const Matrix3d& mat)
{
    const double mat_arr[16] =
    {
        mat.M11, mat.M12, mat.M13, mat.M14,
        mat.M21, mat.M22, mat.M23, mat.M24,
        mat.M31, mat.M32, mat.M33, mat.M34,
        mat.M41, mat.M42, mat.M43, mat.M44,
    };

    glMultMatrixd(mat_arr);
}
void glLoadMatrixd(const Matrix3d& mat)
{
    const double mat_arr[16] =
    {
        mat.M11, mat.M12, mat.M13, mat.M14,
        mat.M21, mat.M22, mat.M23, mat.M24,
        mat.M31, mat.M32, mat.M33, mat.M34,
        mat.M41, mat.M42, mat.M43, mat.M44,
    };

    glLoadMatrixd(mat_arr);
}

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
void glVertex(const vector<Point3d> pnt)
{
    for (auto it = pnt.begin(); it != pnt.end(); ++it)
        glVertex3d(*it);
}

// 回転中心を指定して回転
void RotateAt(const double* mat, const Point3d center)
{
    // 原点に戻して回転する(行列の掛け算は逆!)
    glTranslated(center.X, center.Y, center.Z);
    glMultMatrixd(mat);
    glTranslated(-center.X, -center.Y, -center.Z);
}

// 現在のビューポートを取得します
void GetCurrentViewPort(std::unique_ptr<int>& viewPort)
{
    int* array = new int[4];

    glGetIntegerv(GL_VIEWPORT, array);
    viewPort = make_unique<int>(*array);
}
// 現在のプロジェクション行列を取得します
void GetCurrentProjectionMatrix(std::unique_ptr<double>& projection)
{
    double* array = new double[16];

    glGetDoublev(GL_PROJECTION_MATRIX, array);
    projection = make_unique<double>(*array);
}
// 現在のモデルビュー行列を取得します
void GetCurrentModelviewMatrix(std::unique_ptr<double>& modelview)
{
    double* array = new double[16];

    glGetDoublev(GL_MODELVIEW_MATRIX, array);
    modelview = make_unique<double>(*array);
}

// ワールド座標をローカル座標へ変換する
vector<Point3d> GetLocalCoord(int size, const Point3d* const pnt)
{
    std::vector<Point3d> local;

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

    //    local.push_back(Point3d(winX, winY, winZ));
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

        local.push_back(Point3d(winX, winY, winZ));
    }

    return local;
}

