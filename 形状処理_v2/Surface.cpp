#include "Surface.h"
#include "BsplineCurve.h"

// ����_�ݒ�
void Surface::SetControlPoint(const ControlPoint* const cp, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

    //_ctrlp.reserve(size); _ctrlpX.reserve(size);
    //_ctrlpY.reserve(size); _ctrlpZ.reserve(size);
    //_weight.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);

    // �v�Z�p�Ɋe��������U��
    for (int i = 0; i < _ncpntU; i++)
    {
        for (int j = 0; j < _ncpntV; j++)
        {
            _ctrlpX.emplace_back(_ctrlp[i * _ncpntV + j].X);
            _ctrlpY.emplace_back(_ctrlp[i * _ncpntV + j].Y);
            _ctrlpZ.emplace_back(_ctrlp[i * _ncpntV + j].Z);
            _weight.emplace_back(_ctrlp[i * _ncpntV + j].W);
        }
    }
}

// �`��p�ȗ��x�N�g���擾
Vector3d Surface::GetCurvatureVector(const double u, const double v) const
{
    // ��ȗ��擾
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    // �P�ʖ@���x�N�g��
    Vector3d e = GetNormalVector(u, v).Normalize();

    // ��Βl���傫������Ԃ�
    return (fabs(max_kappa) > fabs(min_kappa)) ?
        (1 / max_kappa) * e :
        (1 / min_kappa) * e;
}

// ��[i=1tok]��[j=1toL] Q(i,j)N[i,n](u)N[j,m](v) ���v�Z����
// <!>��L���Ȗʂ̈ʒu�x�N�g��, �L���Ȗʂ̕��q
Vector3d Surface::CalcVectorWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const
{
    Vector3d retVec;
    double temp[100]; // �v�Z�p

    vector<double> ctrlpX_w, ctrlpY_w, ctrlpZ_w;

    // �������W�ϊ����s��
    for (int i = 0; i < _ncpntU * _ncpntV; i++)
    {
        ctrlpX_w.push_back(_ctrlpX[i] * _weight[i]);
        ctrlpY_w.push_back(_ctrlpY[i] * _weight[i]);
        ctrlpZ_w.push_back(_ctrlpZ[i] * _weight[i]);
    }

    // �s��v�Z�͋t����I
    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &ctrlpX_w[0], temp);
    retVec.X = MatrixMultiply(_ncpntU, temp, BF_array_U);

    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &ctrlpY_w[0], temp);
    retVec.Y = MatrixMultiply(_ncpntU, temp, BF_array_U);

    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &ctrlpZ_w[0], temp);
    retVec.Z = MatrixMultiply(_ncpntU, temp, BF_array_U);

    return retVec;
}

// ��[i=1tok]��[j=1toL] w(i,j)N[i,n](u)N[j,m](v) ���v�Z����
// <!>�L���Ȗʂ̕���
double Surface::CalcWeightWithBasisFunctions(const double* const BF_array_U, const double* const BF_array_V) const
{
    double temp[100]; // �v�Z�p

    // �s��v�Z�͋t����I
    MatrixMultiply(1, _ncpntV, _ncpntU, BF_array_V, &_weight[0], temp);
    return MatrixMultiply(_ncpntU, temp, BF_array_U);
}

// �w�肵���[�̋Ȑ��̐���_���擾����
vector<ControlPoint> Surface::GetEdgeCurveControlPoint(const SurfaceEdge edge) const
{
    vector<ControlPoint> edge_cp;

    if (edge == SurfaceEdge::U_min)
    {
        for (int i = 0; i <_ncpntU * _ncpntV; i += _ncpntU)
            edge_cp.push_back(_ctrlp[i]);
    }
    else if (edge == SurfaceEdge::U_max)
    {
        for (int i = _ncpntU - 1; i < _ncpntU * _ncpntV; i += _ncpntU)
            edge_cp.push_back(_ctrlp[i]);
    }
    else if (edge == SurfaceEdge::V_min)
    {
        for (int i = 0; i < _ncpntU; i++)
            edge_cp.push_back(_ctrlp[i]);
    }
    else if (edge == SurfaceEdge::V_max)
    {
        for (int i = _ncpntU * (_ncpntV - 1) - 1; i < _ncpntU * _ncpntV; i++)
            edge_cp.push_back(_ctrlp[i]);
    }

    return edge_cp;
}

// �w��p�����[�^�Œ�̃A�C�\�Ȑ����擾����
std::unique_ptr<Curve> Surface::GetIsoCurve(const ParamUV const_param, const double param, const GLdouble* const color, const GLdouble width) const
{
    vector<Vector3d> pnts; // �A�C�\�Ȑ��擾�p�̎Q�Ɠ_�Q
    const int split = 30;

    if (const_param == ParamUV::U)
    {
        double skipV = (_max_draw_param_V - _min_draw_param_V) / split;

        for (int i = 0; i <= split; ++i)
            pnts.push_back(GetPositionVector(param, _min_draw_param_V + skipV * i));
    }
    else
    {
        double skipU = (_max_draw_param_U - _min_draw_param_U) / split;

        for (int i = 0; i <= split; ++i)
            pnts.push_back(GetPositionVector(_min_draw_param_U + skipU * i, param));
    }

    return GetBsplineCurveFromPoints(pnts, 4, color, width);
}

// ��ȗ����擾
void Surface::GetPrincipalCurvatures(const double u, const double v, double* const max_kappa, double* const min_kappa) const
{
    Vector3d e = GetNormalVector(u, v).Normalize(); // �P�ʖ@���x�N�g��

    // �e���K�v�Ȓl���v�Z
    double L = e.Dot(GetSecondDiffVectorUU(u, v));
    double M = e.Dot(GetSecondDiffVectorUV(u, v));
    double N = e.Dot(GetSecondDiffVectorVV(u, v));
    double E = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorU(u, v));
    double F = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorV(u, v));
    double G = GetFirstDiffVectorV(u, v).Dot(GetFirstDiffVectorV(u, v));

    double A = E * G - F * F;
    double B = 2 * F * M - E * N - G * L;
    double C = L * N - M * M;

    // ��ȗ��Ȃ��v�Z����iA��^2 + B�� + C = 0 ��2���j
    double kappa1, kappa2;
    SolveQuadraticEquation(A, B, C, &kappa1, &kappa2);

    // �ő��ȗ��ƍŏ���ȗ���Ԃ�
    *max_kappa = (kappa1 > kappa2) ? kappa1 : kappa2;
    *min_kappa = (kappa1 < kappa2) ? kappa1 : kappa2;
}

// ���ϋȗ��擾
double Surface::GetMeanCurvature(const double u, const double v) const
{
    // ��ȗ��擾
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    return (max_kappa + min_kappa) / 2.0;
}

// �K�E�X�ȗ��擾
double Surface::GetGaussianCurvature(const double u, const double v) const
{
    // ��ȗ��擾
    double max_kappa, min_kappa;
    GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

    return max_kappa * min_kappa;
}

// �I�u�W�F�N�g�`��
void Surface::Draw() const
{
    if (isUseLight) // �Ȗʂɂ̓��C�e�B���O�������{��
        glEnable(GL_LIGHTING);

    // ���̕`��
    Object::Draw();

    glDisable(GL_LIGHTING);
}

// ����_���`��
void Surface::DrawCPsInternal() const
{
    //glColor3d(1.0, 0.0, 0.0); // ��
    glColor3dv(_color);
    glPointSize(5.0);
    glLineWidth(1.0);

    // �_�Q
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < _ctrlp.size(); i++)
        glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
    glEnd();

    // U�������Q
    for (int i = 0; i < _ncpntV; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < _ncpntU; j++)
            glVertex3d(_ctrlp[i * _ncpntU + j].X, _ctrlp[i * _ncpntU + j].Y, _ctrlp[i * _ncpntU + j].Z);
        glEnd();
    }
    // V�������Q
    for (int i = 0; i < _ncpntU; i++)
    {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < _ncpntV; j++)
            glVertex3d(_ctrlp[j * _ncpntU + i].X, _ctrlp[j * _ncpntU + i].Y, _ctrlp[j * _ncpntU + i].Z);
        glEnd();
    }
}

// �`��͈͂��e����split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����(pnts[v][u]�ƕ��ׂ�)
void Surface::GetPositionVectors(vector<vector<Vector3d>>& pnts, const int U_split_num, const int V_split_num) const
{
    // �_�Q�̃N���A
    for (auto& pntRow : pnts)
        pntRow.clear();
    pnts.clear();

    vector<Vector3d> pos;

    // ������Ԃ��v�Z
    double skip_U = (fabs(_min_draw_param_U) + fabs(_max_draw_param_U)) / U_split_num;
    double skip_V = (fabs(_min_draw_param_V) + fabs(_max_draw_param_V)) / V_split_num;

    // double�^�̌덷�l��
    for (double v = _min_draw_param_V; v < _max_draw_param_V + skip_V / 2; v += skip_V)
    {
        for (double u = _min_draw_param_U; u < _max_draw_param_U + skip_U / 2; u += skip_U)
            pos.push_back(GetPositionVector(u, v));

        pnts.push_back(pos);
        pos.clear();
    }
}

// �Q�Ɠ_����̍ŋߓ_���擾
NearestPointInfoS Surface::GetNearestPointInfoInternal(const Vector3d& ref, const vector<vector<Point3dS>>& startPnts, const NearestSearch search) const
{
    if (search == Project)
    {
        vector<NearestPointInfoS> possiblePnts; // �ŋߌ��_

        // �J�n�_���̍ŋߓ_���擾�����_�Ƃ���
        for (const auto& startPntRow : startPnts)
        {
            for (const auto& startPnt : startPntRow)
                possiblePnts.push_back(this->GetNearestPointFromRefByProjectionMethod(ref, startPnt));
        }

        // ���̒��ň�ԋ����̒Z�����̂��ŋߓ_�Ƃ���
        NearestPointInfoS nearestPnt(Vector3d(), Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0, 0);
        for (const auto& p : possiblePnts)
        {
            if (p.dist < nearestPnt.dist)
                nearestPnt = p;
        }

        return nearestPnt;
    }
    else if (search == Isoline)
    {
        // �e�J�n�_�̓�, �Ώۓ_�Ƃ̋������ŒZ�̂��̂��J�n�_�Ƃ���
        double dist;
        double min_dist = DBL_MAX;
        Point3dS start(Vector3d(DBL_MAX, DBL_MAX, DBL_MAX), 0, 0);

        for (const auto& startPntRow : startPnts)
        {
            for (const auto& startPnt : startPntRow)
            {
                dist = ref.DistanceFrom((Vector3d)const_cast<Point3dS &>(startPnt));
                if (dist < min_dist)
                {
                    min_dist = dist;
                    start = startPnt;
                }
            }
        }

        // �A�C�\���C���@�͓K�؂ȊJ�n�_�̑I����1���ōŋߓ_�����肷��
        return this->GetNearestPointFromRefByIsolineMethod(ref, start);
    }
    else
        throw;
}
// �ŋߓ_���擾����(�ˉe�@)
// �R�����g����e_.�͎����̏I�������ԍ�
NearestPointInfoS Surface::GetNearestPointFromRefByProjectionMethod(const Vector3d& ref, const Point3dS& start) const
{
    int count = 0; // �X�e�b�v��

    // �����p�����[�^
    double u = start.paramU;
    double v = start.paramV;

    Vector3d p; // �ʒu�x�N�g��
    Vector3d pu; // U�����ڐ��x�N�g��
    Vector3d pv; // V�����ڐ��x�N�g��
    double delta_u; // U�����p�����[�^�ړ���
    double delta_v; // V�����p�����[�^�ړ���

    // �X�V
    auto update = [&]()
    {
        // �e�x�N�g���X�V
        p = GetPositionVector(u, v);
        pu = GetFirstDiffVectorU(u, v);
        pv = GetFirstDiffVectorV(u, v);
        // �p�����[�^�ړ��ʍX�V
        delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.7;
        delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.7;
    };

    update(); // �����X�V

    while (true)
    {
        u += delta_u;
        v += delta_v;

        // �p�����[�^���`��͈͂���͂ݏo���ꍇ
        if (u < _min_draw_param_U || u > _max_draw_param_U ||
            v < _min_draw_param_V || v > _max_draw_param_V)
            return this->GetNearestPointWhenParamOver(ref, u, v);

        update(); // �X�V

        // e1. ���ڃx�N�g����Pu������ ���� ���ڃx�N�g����Pv������
        if (fabs((ref - p).Dot(pu)) < EPS::NEAREST && fabs((ref - p).Dot(pv) < EPS::NEAREST))
            break;
        // e2. �����W�̈ړ��ʂ�U,V�����Ƃ���0
        if (GetPositionVector(u, v).DistanceFrom(GetPositionVector(u - delta_u, v)) < EPS::NEAREST &&
            GetPositionVector(u, v).DistanceFrom(GetPositionVector(u, v - delta_v)) < EPS::NEAREST)
            break;
        // e3. �p�����[�^�ړ��ʂ�U,V�����Ƃ���0
        if (fabs(delta_u) < EPS::NEAREST && fabs(delta_v) < EPS::NEAREST)
            break;
        // e4. �X�e�b�v�����
        if (++count > EPS::COUNT_MAX)
            break;
        // e5. �Ȗʏ�̓_
        if (ref.DistanceFrom(p) < EPS::DIST)
            break;
    }

    return NearestPointInfoS(p, ref, u, v);
}
// �p�����[�^���͂ݏo�����Ƃ��̍ŋߓ_�擾(�Ȗʂ̒[�ɍŋߓ_������͂�)
// u,v�������Ƃ��ɂ͂ݏo���ꍇ��OK
NearestPointInfoS Surface::GetNearestPointWhenParamOver(const Vector3d& ref, const double u, const double v) const
{
    std::unique_ptr<Curve> edge; // �Ȗʂ̒[
    std::unique_ptr<NearestPointInfoC> nearInfo; // �ŋߓX���

    // u�������͂ݏo��ꍇ
    if (u < _min_draw_param_U || u > _max_draw_param_U)
    {
        if (u < _min_draw_param_U)
        {
            edge = GetEdgeCurve(SurfaceEdge::U_min);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, _min_draw_param_U, nearInfo->param);
        }
        else
        {
            edge = GetEdgeCurve(SurfaceEdge::U_max);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, _max_draw_param_U, nearInfo->param);
        }
    }
    // v�������͂ݏo��ꍇ
    else if (v < _min_draw_param_V || v > _max_draw_param_V)
    {
        if (v < _min_draw_param_V)
        {
            edge = GetEdgeCurve(SurfaceEdge::V_min);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, nearInfo->param, _min_draw_param_V);
        }
        else
        {
            edge = GetEdgeCurve(SurfaceEdge::V_max);
            nearInfo = std::make_unique<NearestPointInfoC>(edge->GetNearestPointInfoFromRef(ref));
            return NearestPointInfoS(nearInfo->nearestPnt, ref, nearInfo->param, _max_draw_param_V);
        }
    }

    throw;
}
// �ŋߓ_���擾����(�A�C�\���C���@)
// �R�����g����e_�͎����̏I�������ԍ�
NearestPointInfoS Surface::GetNearestPointFromRefByIsolineMethod(const Vector3d& ref, const Point3dS& start) const
{
    int count = 0; // �X�e�b�v��
    std::unique_ptr<Curve> iso; // �A�C�\�Ȑ�
    std::unique_ptr<NearestPointInfoC> nearInfo; // �ŋߓ_���
    double u, v; // ���݂̃p�����[�^
    double end_param; // �I���_�̃p�����[�^
    double dot; // ���ϒl

    double ori_rangeU = _max_draw_param_U - _min_draw_param_U;
    double ori_rangeV = _max_draw_param_V - _min_draw_param_V;

    // �����X�V
    u = start.paramU;
    v = start.paramV;
    iso = this->GetIsoCurve(ParamUV::V, v);
    dot = (ref - (Vector3d)const_cast<Point3dS &>(start)).Dot(GetFirstDiffVectorU(u, v));
    if (dot > 0)
        end_param = _max_draw_param_U;
    else
        end_param = _min_draw_param_U;

    while (true)
    {
        // �ŋߓ_���擾
        if (count % 2 == 0)
        {
            if (u < end_param)
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, u / ori_rangeU, end_param / ori_rangeU, 8));
            else
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, end_param / ori_rangeU, u / ori_rangeU, 8));
        }
        else
        {
            if (v < end_param)
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, v / ori_rangeV, end_param / ori_rangeV, 8));
            else
	      nearInfo = std::make_unique<NearestPointInfoC>(iso->GetSectionNearestPointInfoByBinary(ref, end_param / ori_rangeV, v / ori_rangeV, 8));
        }
        ++count;

        // U, V�����̃A�C�\�Ȑ������݂Ɏ��
        if (count % 2 == 0) // U����
        {
            v = nearInfo->param * ori_rangeV;
            iso = this->GetIsoCurve(ParamUV::V, v);

            dot = (ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorU(u, v));
            if (dot > 0)
                end_param = _max_draw_param_U;
            else
                end_param = _min_draw_param_U;
        }
        else // V����
        {
            u = nearInfo->param * ori_rangeU;
            iso = this->GetIsoCurve(ParamUV::U, u);

            dot = (ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorV(u, v));
            if (dot > 0)
                end_param = _max_draw_param_V;
            else
                end_param = _min_draw_param_V;
        }

        // e1. �x�N�g���̓��ς�U,V�����Ƃ��ɒ���
        if ((fabs((ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorU(u, v))) < EPS::NEAREST) &&
            (fabs((ref - nearInfo->nearestPnt).Dot(GetFirstDiffVectorV(u, v))) < EPS::NEAREST))
            break;
        if (count > 1) // 1��ڂ͒[���ŋߓ_����V�����ɓ����Ȃ�����ړ������͊O��
        {
            // e2. �����W�̈ړ��ʂ�U,V�����Ƃ���0
            if (GetPositionVector(u, v).DistanceFrom(GetPositionVector(u - nearInfo->param * ori_rangeU, v)) < EPS::NEAREST &&
                GetPositionVector(u, v).DistanceFrom(GetPositionVector(u, v - nearInfo->param * ori_rangeV)) < EPS::NEAREST)
                break;
            // e3. �p�����[�^�ړ��ʂ�U,V�����Ƃ���0
            if (fabs(u - nearInfo->param * ori_rangeU) < EPS::NEAREST && fabs(v - nearInfo->param * ori_rangeV) < EPS::NEAREST)
                break;
        }
        // e4. �X�e�b�v�����
        if (count > EPS::COUNT_MAX)
            break;
        // e5. �Ȗʏ�̓_
        if (ref.DistanceFrom(nearInfo->nearestPnt) < EPS::DIST)
            break;
    }

    return NearestPointInfoS(nearInfo->nearestPnt, ref, u, v);
}
