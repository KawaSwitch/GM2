#include "Curve.h"
#include <cfloat>

// ����_�ݒ�
void Curve::SetControlPoint(const ControlPoint* const cp, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

    _ctrlp.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);
}

// ����_�`��
void Curve::DrawCPsInternal() const
{
    glColor3d(1.0, 0.0, 0.0); // ��
    glPointSize(10.0);
    glLineWidth(1.0);

    // �_�Q
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < _ctrlp.size(); i++)
        glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
    glEnd();

    // ���Q
    glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i < _ctrlp.size(); i++)
        glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
    glEnd();
}

// �ȗ��x�N�g���擾
Vector3d Curve::GetCurvatureVector(const double t) const
{
    double kappa =  // �� = |Pt�~Ptt| / |Pt|^3
        (GetFirstDiffVector(t) * GetSecondDiffVector(t)).Length() // |Pt�~Ptt|
        / pow(GetFirstDiffVector(t).Length(), 3); // |Pt|^3

    // �@������N = (Pt �~ Ptt) �~ Pt
    Vector3d direct = (GetFirstDiffVector(t) * GetSecondDiffVector(t)) * GetFirstDiffVector(t);

    return (1 / kappa) * direct.Normalize();
}

// �`��͈͂�split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����
void Curve::GetPositionVectors(vector<Vector3d>& pnts, const int split_num) const
{
    pnts.clear();

    // ������Ԃ��v�Z
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split_num;

    // double�^�̌덷�l��
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        pnts.push_back(GetPositionVector(t));
}

// ���Ȑ��Ƃ̑���x���v�Z���܂�
// NOTE: ���݃m�b�g�ʒu�̕��z�ɕ΂肪����Ƌ@�\���Ȃ�
double Curve::CalcDifferency(const Curve* const other) const
{
    int checkCnt = 100; // �����𑪂�_�̐�
    double sumDistance = 0.0; // ���዗���̍��v

    // ������Ԃ��v�Z
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / checkCnt;

    // double�^�̌덷�l��
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        sumDistance += this->GetPositionVector(t).DistanceFrom(other->GetPositionVector(t / this->GetDrawParamRange()));

    // ���዗���̕��ς�Ԃ�
    return sumDistance / (double)checkCnt;
}
// ���Ȑ��Ƃ̑���x���v�Z���܂�
// �ŋߓ_����
double Curve::CalcDifferency2(const Curve* const other) const
{
    int checkCnt = 100; // �����𑪂�_�̐�
    double sumDistance = 0.0; // ���዗���̍��v

    // �Q�Ɠ_�Q���擾
    vector<Vector3d> ref_pnts;
    other->GetPositionVectors(ref_pnts, checkCnt - 1);

    // �ŋߓ_�擾
    vector<NearestPointInfoC> nearest_pnts;
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(this->GetNearestPointInfoFromRef(ref_pnts[i]));

    for (const auto& p : nearest_pnts)
        sumDistance += p.dist;

    // ���዗���̕��ς�Ԃ�
    return sumDistance / (double)checkCnt;
}

// 2�Ȑ��ň�ԉ������鋗�����v�Z����
double Curve::CalcFarthestDistant(const Curve* const other) const
{
    int checkCnt = 100; // �����𑪂�_�̐�
    double farthestDist = -DBL_MAX; // 2�Ȑ��ň�ԉ�������

    // �Q�Ɠ_�Q���擾
    vector<Vector3d> ref_pnts;
   other->GetPositionVectors(ref_pnts, checkCnt - 1);

    // �ŋߓ_�擾
    vector<NearestPointInfoC> nearest_pnts;
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(this->GetNearestPointInfoFromRef(ref_pnts[i]));

    for (const auto& p : nearest_pnts)
    {
        if (p.dist > farthestDist)
            farthestDist = p.dist;
    }

    return farthestDist;
}

// �Ȑ������擾���܂�
// split : ������
double Curve::GetLength(int split) const
{
    double dist = 0.0; // �Ȑ���

    // ������Ԃ��v�Z
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split;

    // double�^�̌덷�l��
    for (double t = _min_draw_param; t < _max_draw_param - skip - skip / 2; t += skip)
        dist += this->GetPositionVector(t).DistanceFrom(this->GetPositionVector(t + skip));

    return dist;
}

// �Q�Ɠ_����̍ŋߓ_����2���T���Ŏ擾���܂�
// ref : �Q�Ɠ_, startPnts : 2���T���J�n�_�ʒu���
NearestPointInfoC Curve::GetNearestPointInfoInternal(const Vector3d& ref, const vector<Point3dC>& startPnts) const
{
    vector<NearestPointInfoC> possiblePnts; // �ŋߌ��_
    double left, right; // 2���T���p�p�����[�^

    // �J�n�_�͈͖��̍ŋߓ_���擾�����_�Ƃ���
    for (size_t i = 0, s = startPnts.size(); i < s; ++i)
    {
        left = startPnts[(i == 0) ? i : i - 1].param;
        right = startPnts[(i == s - 1) ? i : i + 1].param;

        possiblePnts.push_back(GetSectionNearestPointInfoByBinary(ref, left, right));
    }

    // ���̒��ň�ԋ����̒Z�����̂��ŋߓ_�Ƃ���
    NearestPointInfoC nearestPnt(Vector3d(), Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0);
    for (const auto& p : possiblePnts)
    {
        if (p.dist < nearestPnt.dist)
            nearestPnt = p;
    }

    return nearestPnt;
}

// ��ԓ��ŋߓ_�擾(2���T��)
// �R�����g����e_�͎����̏I�������ԍ�
NearestPointInfoC Curve::GetSectionNearestPointInfoByBinary(const Vector3d& ref, const double ini_left, const double ini_right) const
{
    Vector3d pnt, vec_ref_pnt, tan;
    double left, right, middle; // 2���T���p�p�����[�^
    double dot; // ���ϒl
    int count = 0; // 2���T���X�e�b�v��

    left = ini_left; right = ini_right;

    auto update = [&]()
    {
        middle = (left + right) / 2;
        pnt = GetPositionVector(middle);
        tan = GetFirstDiffVector(middle);
        vec_ref_pnt = pnt - ref;
        dot = tan.Dot(vec_ref_pnt); // ���ϒl
    };

    // �����X�V
    update();

    while (left <= right)
    {
        // e5. �Q�Ɠ_���ΏۋȐ��̕�������ɂ���(�������W�ƎQ�Ɠ_���d�Ȃ�)
        if (fabs(pnt.DistanceFrom(ref)) < EPS::DIST_SQRT)
            break;

        // e2. �p�����[�^�̈ړ���0
        if (fabs(left - middle) < EPS::NEAREST || fabs(right - middle) < EPS::NEAREST)
            break;
        // e3. �����W�̈ړ���0
        if (fabs(GetPositionVector(left).DistanceFrom(GetPositionVector(middle))) < EPS::NEAREST ||
            fabs(GetPositionVector(right).DistanceFrom(GetPositionVector(middle))) < EPS::NEAREST)
            break;

        // e1. �x�N�g���̓��ς�0
        if (-EPS::NEAREST < dot && dot < EPS::NEAREST)
        {
            // �\���Ȑ��x�Ȃ̂Ō����������Ƃɂ���
            break;
        }
        else if (dot >= EPS::NEAREST)
        {
            // �E�[�X�V
            right = middle;
        }
        else if (dot <= -EPS::NEAREST)
        {
            // ���[�X�V
            left = middle;
        }

        // �e�l�X�V
        update();

        // e4. �X�e�b�v������ɒB�����炻�̎��_�̓_��Ԃ�
        if (++count > EPS::COUNT_MAX)
            break;
    }

    return NearestPointInfoC(pnt, ref, middle);
}
// ��ԓ��ŋߓ_�擾(2���T��)
NearestPointInfoC Curve::GetSectionNearestPointInfoByBinary(const Vector3d& ref, double ini_left, double ini_right, int split) const
{
    vector<NearestPointInfoC> possiblePnts; // �ŋߌ��_
    double skip = (ini_right - ini_left) / split;

    // �p�����[�^�͈͂𕪊����čŋߓ_���擾����
    if (fabs(ini_left - ini_right) < EPS::DIST)
    {
        // ���l�ƉE�l���������ꍇ
        possiblePnts.push_back(GetSectionNearestPointInfoByBinary(ref, ini_left, ini_right));
    }
    else
    {
        for (double param = ini_left; param < ini_right - skip / 2; param += skip)
            possiblePnts.push_back(GetSectionNearestPointInfoByBinary(ref, param, param + skip));
    }

    // ���̒��ň�ԋ����̒Z�����̂��ŋߓ_�Ƃ���
    NearestPointInfoC nearestPnt(Vector3d(), Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0);
    for (const auto& p : possiblePnts)
    {
        if (p.dist < nearestPnt.dist)
            nearestPnt = p;
    }

    return nearestPnt;
}