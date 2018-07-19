#include "BezierCurve.h"
#include "ControlPoint.h"
#include "glUtil.h"

BezierCurve::BezierCurve(const int mord, const ControlPoint* const cp, const int cp_size,
    const GLdouble* const color, const GLdouble width, const double resol)
{
    _ord = mord;
    _ncpnt = cp_size;
    _min_draw_param = 0.0;
    _max_draw_param = 1.0;

    SetControlPoint(cp, cp_size);
    SetColor(color);
    _width = width;
    _resolution = resol;

    _length = this->GetLength();
    _draw_vec_length = _length * _draw_vector_ratio;

    // VBO�g��
    _isUseVBO = true;
}

// ���O�`��
void BezierCurve::PreDraw() const
{
    glColor4dv(_color);
    glLineWidth(_width);

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= 100; i += 1)
    {
        double t = (double)i / 100;
        Vector3d pnt = GetPositionVector(t);

        glVertex3d(pnt);
    }

    glEnd();
}

// ���_�o�b�t�@�쐬
void BezierCurve::CreateVBO() const
{
    vector<Vector3d> pnts;

    // ���_�擾
    for (int i = 0; i <= 100; i += 1)
    {
        double t = (double)i / 100;
        pnts.push_back(GetPositionVector(t));
    }

    _nVertex_cache = (int)pnts.size();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, pnts.size() * 3 * sizeof(double), (GLdouble*)&pnts[0], GL_DYNAMIC_DRAW);
}

// VBO�ŕ`��
void BezierCurve::DrawVBO() const
{
    glColor4dv(_color);
    glLineWidth(_width);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex_cache);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// �ڐ��x�N�g���`��
void BezierCurve::DrawFirstDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetFirstDiffVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 2�K�����x�N�g���`��
void BezierCurve::DrawSecondDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetSecondDiffVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// �@���x�N�g���`��
void BezierCurve::DrawNormalVectorsInternal() const
{
    Vector3d pnt, normal;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        normal = GetNormalVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + normal);
    }

    glEnd();
}

// �ȗ����a�`��
void BezierCurve::DrawCurvatureVectorsInternal() const
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = 0; i <= 100; i += 2)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        curv = GetCurvatureVector(t);

        // �ȗ����a�`��
        glColor4dv(Color::pink);
        glBegin(GL_LINES);
        glVertex3d(pnt);
        glVertex3d(pnt + curv);
        glEnd();

        // �ȗ����S�`��
        glColor4dv(Color::light_green);
        glBegin(GL_POINTS);
        glVertex3d(pnt + curv);
        glEnd();
    }
}

// �ʒu�x�N�g���擾
Vector3d BezierCurve::GetPositionVector(const double t) const
{
    Vector3d pnt;

    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBernsteinFunc(i, _ord - 1, t) * _ctrlp[i];

    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BezierCurve::GetFirstDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBernsteinFunc(i, _ord - 1, t) * _ctrlp[i];

    return diff;
}

// 2�K�����x�N�g���擾
Vector3d BezierCurve::GetSecondDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBernsteinFunc(i, _ord - 1, t) * _ctrlp[i];

    return diff;
}

// �ʉߓ_����t�ϊ����ċȐ����擾
std::unique_ptr<Curve> BezierCurve::GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, const GLdouble width) const
{
    vector<ControlPoint> new_cps;
    new_cps.resize(_ncpnt);

    //double dist0 = pnts[0].GetDistance(sample_pnt[1]);
    //double dist1 = sample_pnt[1].GetDistance(sample_pnt[2]);
    //double dist2 = sample_pnt[2].GetDistance(sample_pnt[3]);

    //// �ʉߓ_�̃T���v���ʒu
    //double sample_t[4] =
    //{
    //    0.0,
    //    dist0 / (dist0 + dist1 + dist2),
    //    (dist0 + dist1) / (dist0 + dist1 + dist2),
    //    1.0,
    //};

    //// �V��������_�����߂�
    //// ���֐��p�s��
    //double **B_matrix = new double*[_ncpnt];
    //for (int i = 0; i < _ncpnt; i++)
    //    B_matrix[i] = new double[_ncpnt];

    //// ���֐��s����쐬
    //for (int i = 0; i < _ncpnt; i++)
    //{
    //    for (int j = 0; j < _ncpnt; j++)
    //        B_matrix[i][j] = CalcBernsteinFunc(j, _ord - 1, )
    //}

    return  std::unique_ptr<Curve>(new BezierCurve(_ord, &new_cps[0], _ncpnt, color, width));
}

// �ŋߓ_�擾
NearestPointInfoC BezierCurve::GetNearestPointInfoFromRef(const Vector3d& ref) const
{
    // NOTE: ������
    return NearestPointInfoC(Vector3d(), ref, 0);
}