#include "BsplineCurve.h"
#include "ControlPoint.h"
#include <iterator>
#include <cfloat>
#include <memory>

BsplineCurve::BsplineCurve(const int mord, const ControlPoint* const cp, const int cp_size, const double* const knot,
    const GLdouble* const color, const GLdouble width, const double resol)
{
    _ord = mord;
    _ncpnt = cp_size;
    _nknot = mord + cp_size;

    _min_draw_param = knot[_ord - 1];
    _max_draw_param = knot[_ncpnt];

    SetControlPoint(cp, cp_size);
    SetKnotVector(knot, _nknot);
    SetColor(color);
    _width = width;
    _resolution = resol;

    _length = this->GetLength();
    _draw_vec_length = _length * _draw_vector_ratio;

    // VBO�g��
    _isUseVBO = true;
}

// �m�b�g�x�N�g���ݒ�
void BsplineCurve::SetKnotVector(const double* const knot, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("�m�b�g�x�N�g���ݒ莸�s", "knot-vector size must be over 0.");

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
// splitSegCnt: �Z�O�����g�����������邩�̉�(�f�t�H���g��1 = �������Ȃ�)
vector<Vector3d> BsplineCurve::GetPositionVectorsByKnots(const int splitSegCnt) const
{
    vector<Vector3d> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (int i = 0; i < _knot.size(); ++i)
    {
        // �m�b�g�̊K���[�̔�`�敔��or�d���͏Ȃ�
        if ((i > 0 && i < _ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(this->GetPositionVector(_knot[i]));

        // �Ō�͒ǉ����Ȃ�
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(this->GetPositionVector(_knot[i] + skip * j));
        }
    }

    return pnts;
}
// �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
// splitSegCnt: �Z�O�����g�����������邩�̉�(�f�t�H���g��1 = �������Ȃ�)
vector<Point3dC> BsplineCurve::GetPointsByKnots(const int splitSegCnt) const
{
    vector<Point3dC> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (int i = 0; i < _knot.size(); ++i)
    {
        // �m�b�g�̊K���[�̔�`�敔��or�d���͏Ȃ�
        if ((i > 0 && i < _ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(Point3dC(this->GetPositionVector(_knot[i]), _knot[i]));

        // �Ō�͒ǉ����Ȃ�
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(Point3dC(this->GetPositionVector(_knot[i] + skip * j), _knot[i] + skip * j));
        }
    }

    return pnts;
}

// ���O�`��
void BsplineCurve::PreDraw() const
{
    Vector3d pnt;

    glColor4dv(_color);
    glLineWidth(_width);
    glBegin(GL_LINE_STRIP);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        glVertex3d(pnt);
    }

    glEnd();
}

// ���_�o�b�t�@�쐬
void BsplineCurve::CreateVBO() const
{
    vector<Vector3d> pnts;

    // ���_�擾
    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
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
void BsplineCurve::DrawVBO() const
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
void BsplineCurve::DrawFirstDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
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
void BsplineCurve::DrawSecondDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
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
void BsplineCurve::DrawNormalVectorsInternal() const
{
    Vector3d pnt, normal;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
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
void BsplineCurve::DrawCurvatureVectorsInternal() const
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
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

    glEnd();
}

// �ʒu�x�N�g���擾
Vector3d BsplineCurve::GetPositionVector(const double t) const
{
    Vector3d pnt;

    // �W���I
    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BsplineCurve::GetFirstDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 2�K�����x�N�g���擾
Vector3d BsplineCurve::GetSecondDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// �ʉߓ_����t�ϊ����ċȐ����擾����(�����o�֐���)
std::unique_ptr<Curve> BsplineCurve::GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, const GLdouble width) const
{
    return GetBsplineCurveFromPoints(pnts, 4, color, width);
}
// �ʊϓ_����t�ϊ����ċȐ����擾����
std::unique_ptr<Curve> GetBsplineCurveFromPoints(const vector<Vector3d>& pnts, int ord, const GLdouble* const color, GLdouble width)
{
    int passPntsCnt = (int)pnts.size(); // �ʉߓ_��
    int new_ncpnt = (passPntsCnt - 1) + (ord - 1); // �V��������_��

    vector<double> new_knots;
    CalcKnotVectorByPassingPnts(pnts, ord, &new_knots);

    vector<ControlPoint> new_cps;
    CalcControlPointsByPassingPnts(pnts, ord, new_knots, &new_cps);

    return std::unique_ptr<BsplineCurve>(new BsplineCurve(ord, &new_cps[0], new_ncpnt, &new_knots[0], color, width));
}

// �Q�Ɠ_����̍ŋߓ_�����擾
NearestPointInfoC BsplineCurve::GetNearestPointInfoFromRef(const Vector3d& ref) const
{
    const int seg_split = 8; // �Z�O�����g������
    auto startPnts = this->GetPointsByKnots(seg_split); // �J�n�_�Q

    return Curve::GetNearestPointInfoInternal(ref, startPnts);
}