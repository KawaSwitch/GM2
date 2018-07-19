#define _USE_MATH_DEFINES

#include <numeric>
#include <vector>
#include "CalcUtil.h"
#include "Matrix.h"
#include "Error.h"
using std::vector;

// �x���@����ʓx�@�ɕϊ�����
double ToRad(const double degree)
{
    constexpr double C = M_PI / 180;
    return degree * C;
}
// �ʓx�@����x���@�ɕϊ�����
double ToDeg(const double radian)
{
    constexpr double C = 180 / M_PI;
    return radian * C;
}

// ���ϒl���擾����
double GetAverage(const vector<double>& array)
{
    return std::accumulate(array.begin(), array.end(), 0.0) / array.size();
}

// 2�������� ax^2 + bx + c = 0 �� 2��x1,x2 �����߂�
// �������̏ꍇ x1 �} x2i �𖞂��� x1,x2 �����߂�
void SolveQuadraticEquation(const double a, const double b, const double c, double* const x1, double* const x2)
{
    // ���ʎ�
    double D = b * b - 4 * a * c;

    // �قȂ�2�̎�����
    if (D > 0)
    {
        *x1 = (-b + sqrt(D)) / (2 * a);
        *x2 = (-b - sqrt(D)) / (2 * a);
    }
    // �d��
    else if (D == 0)
    {
        // �d���̂Ƃ� x = -(b / 2a)
        *x1 = *x2 = -b / (2 * a);
    }
    // �قȂ�2�̋�����
    else
    {
        *x1 = -b / (2 * a); // real part
        *x2 = sqrt(-D) / (2 * a); // imaginary part
    }
}

// �񍀌W�������߂�
constexpr double Binomial(const int n, const int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

// �K��l���v�Z����
constexpr int Factorial(const int n)
{
    return (n == 0) ? 1 : n * Factorial(n - 1);
}

// �o�[���X�^�C�����֐������߂�
double CalcBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    return Binomial(N, i) * pow(t, i) * pow(1 - t, N - i);
}

// 1�K�����p�o�[���X�^�C�����֐������߂�
double Calc1DiffBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    return Binomial(N, i) * (i * pow(t, i - 1) * pow(1 - t, N - i) - (N - i) * pow(t, i) * pow(1 - t, N - i - 1));
}

// 2�K�����p�o�[���X�^�C�����֐������߂�
double Calc2DiffBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    double temp_left = i * ((i - 1) * pow(t, i - 2) * pow(1 - t, N - i)
        - pow(t, i - 1) * (N - i) * pow(1 - t, N - i - 1));

    double temp_right = (N - i) * (i * pow(t, i - 1) * pow(1 - t, N - i - 1)
        - pow(t, i) * (N - i - 1) * pow(1 - t, N - i - 2));

    return Binomial(N, i) * (temp_left - temp_right);
}

// B�X�v���C�����֐��l�����߂�
double CalcBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0 , temp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = ((t - knot[i]) / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = ((knot[i + M] - t) / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return temp_left + temp_right;
    }
}

// 1�K�����pB�X�v���C�����֐��l�����߂�
double Calc1DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0, temp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// 2�K�����pB�X�v���C�����֐��l�����߂�
double Calc2DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0, temp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * Calc1DiffBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * Calc1DiffBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// �ʉߓ_���������Z�b�g����Ă��邩
// U����/V�����̓_�̐������ꂼ�ꓙ���������`�F�b�N����
bool IsPassingPntsSetCorrectly(const vector<vector<Vector3d>>& pnts)
{
    if (pnts.size() < 1)
        return false;

    // ����(u����)�������ׂ�΂悢
    for (const auto& u_pnts : pnts)
    {
        if (pnts[0].size() != u_pnts.size())
            return false;
    }

    // ���ׂē������Ȃ琳����(n�~m��)
    return true;
}

// �ʉߓ_�ƊK������m�b�g�x�N�g�����Z�o����
void CalcKnotVectorByPassingPnts(const vector<Vector3d>& pnts, const int ord, vector<double>* knot)
{
    if (knot == nullptr)
    {
        Error::ShowAndExit("�k���|�C���^");
        return;
    }

    // �m�b�g��T�C�Y�͐���_���ƊK���̘a
    knot->resize((pnts.size() + ord - 2) + ord);

    vector<double> seg_dist; // �Z�O�����g�Ԃ̋���
    seg_dist.resize(pnts.size() - 1);

    double minParam = 0.0;
    double maxParam = 1.0;
    double paramRange = fabs(maxParam - minParam);

    // �ʉߓ_�Ԃ̋����̑��a
    double sum = 0.0;
    for (size_t i = 0, n = pnts.size(); i < n - 1; ++i)
    {
        seg_dist[i] = pnts[i].DistanceFrom(pnts[i + 1]);
        sum += seg_dist[i];
    }

    // �m�b�g�Ԋu�̊����͒ʉߓ_�Ԃ̋����ɔ�Ⴓ����
    for (size_t i = 0, n = knot->size(); i < n; ++i)
    {
        if (i < ord) // �ŏ��͊K�����d�˂�
            (*knot)[i] = minParam;
        else if (i < knot->size() - ord) // �����ɔ��
        {
            (*knot)[i] = (*knot)[i - 1] + (seg_dist[i - ord] / sum) * paramRange;
        }
        else // �Ō���K�����d�˂�
            (*knot)[i] = maxParam;
    }
}

// �����̃m�b�g��𐳋K������
void NormalizeKnotVector(const vector<vector<double>>& knot_array, const int ord, vector<double>* knot_normal)
{
    if (knot_normal == nullptr)
    {
        Error::ShowAndExit("�k���|�C���^");
        return;
    }
    if (knot_array.size() < 1)
    {
        Error::ShowAndExit("��w��");
        return;
    }

    // ���K���O��0�Ԗڂ̃m�b�g��ŏ��������Ă���
    knot_normal->resize(knot_array[0].size());
    for (size_t i = 0, n = knot_array[0].size(); i < n; ++i)
        (*knot_normal)[i] = knot_array[0][i];

    // �K�v�Ȍ�(�K������)�𐳋K������
    for (size_t col = ord, n = knot_array[0].size(); col < n - ord; ++col)
    {
        double sum = 0.0;

        for (size_t row = 0, n = knot_array.size(); row < n; ++row)
            sum += knot_array[row][col];

        // ����
        (*knot_normal)[col] = sum / knot_array.size();
    }
}

// �m�b�g�x�N�g�����擾����
void CalcKnotVectorByPassingPnts(const vector<vector<Vector3d>>& pnts, const int ordU, const int ordV,
    vector<double>* knotU, vector<double>* knotV)
{
    // �e�����̒ʉߓ_��
    int passPntsCntU, passPntsCntV;
    passPntsCntU = (int)pnts[0].size();
    passPntsCntV = (int)pnts.size();

    // �e�����̒ʉߓ_����擾����m�b�g��Q
    vector<vector<double>> new_knotsU(passPntsCntV), new_knotsV(passPntsCntU);

    // U�����m�b�g�x�N�g���̌���
    {
        for (int i = 0; i < passPntsCntV; ++i)
        {
            vector<Vector3d> pntsU(passPntsCntU);
            vector<double> knot;

            for (int j = 0; j < passPntsCntU; ++j)
                pntsU[j] = pnts[i][j];

            CalcKnotVectorByPassingPnts(pntsU, ordU, &knot);
            new_knotsU[i] = knot;
        }
        // ���K��
        NormalizeKnotVector(new_knotsU, ordU, knotU);
    }
    // V�����m�b�g�x�N�g���̌���
    {
        for (int i = 0; i < passPntsCntU; ++i)
        {
            vector<Vector3d> pntsV(passPntsCntV);
            vector<double> knot;

            for (int j = 0; j < passPntsCntV; ++j)
                pntsV[j] = pnts[j][i];

            CalcKnotVectorByPassingPnts(pntsV, ordV, &knot);
            new_knotsV[i] = knot;
        }
        // ���K��
        NormalizeKnotVector(new_knotsV, ordV, knotV);
    }
}

// 1�����̓_�Q���炻����ʂ�Ȑ��̐���_���Z�o����
/*
�ʉߓ_�����ƏI�[��������A�����������쐬������
������, �ʉߓ_��n ����_��qn(n + �K�� - 2) �m�b�g��T�C�Ykn �Ƃ���

[�ʉߓ_����]
<�[�_����>
P(k_0) = Q_0
P(k_kn-1) = Q_qn-1
<����ȊO�̒ʉߓ_����>
P(k_�K��) = Q_1
P(k_�K��+1) = Q_2
...
P(k_kn-�K��-1) = Q_qn-2

[�I�[����]
P''(k_�K��-1) = 0
P''(k_kn-�K��-2) = 0
*/
void CalcControlPointsByPassingPnts(const vector<Vector3d>& pnts, const int ord, const vector<double>& knot, vector<ControlPoint>* ctrlp)
{
    // �e�����̒ʉߓ_��
    int passPntsCnt = (int)pnts.size();
    // �V��������_��
    int new_ncpnt = (passPntsCnt - 1) + (ord - 1);

    // �ʉߓ_�x�N�g��(�A���������p)
    vector<Vector3d> P_array;
    vector<double> P_array_x, P_array_y, P_array_z; // �ˉe��

                                                    // ���֐��s��
    double* N_matrix;

    // 1. �A���������������p�̒ʉߓ_�x�N�g�� �쐬
    {
        P_array.push_back(pnts[0]);
        P_array.push_back(Vector3d::Zero());
        for (int i = 1; i < passPntsCnt - 1; ++i)
            P_array.push_back(pnts[i]);
        P_array.push_back(Vector3d::Zero());
        P_array.push_back(pnts[passPntsCnt - 1]);

        // �v�Z�p�Ɋe�v�f�Ɏˉe
        for (auto it = P_array.begin(), end = P_array.end(); it != end; ++it)
        {
            P_array_x.push_back((*it).X);
            P_array_y.push_back((*it).Y);
            P_array_z.push_back((*it).Z);
        }
    }

    // 2. ���֐��p�s�� �쐬
    {
        N_matrix = new double[new_ncpnt * new_ncpnt];

        // ��s��ɏ�����
        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix[i * new_ncpnt + j] = 0;
        }

        for (int i = 0; i < new_ncpnt; i++)
        {
            if (i == 0) // �I�[���� �O
            {
                // ��ԍ��̂�1
                N_matrix[i * new_ncpnt + 0] = 1;
            }
            else if (i == 1) // 2�K�����I�[���� �O
            {
                for (int j = i - 1; j < ord - 1; ++j)
                    N_matrix[i * new_ncpnt + j] = Calc2DiffBsplineFunc(j, ord, knot[ord - 1], &(const_cast<vector<double>&>(knot))[0]);
            }
            else if (i > 1 && i < new_ncpnt - 2)
            {
                for (int j = i - 1; j < (ord - 1) + (i - 1); ++j)
                    N_matrix[i * new_ncpnt + j] = CalcBsplineFunc(j, ord, knot[(ord - 1) + (i - 1)], &(const_cast<vector<double>&>(knot))[0]);
            }
            else if (i == new_ncpnt - 2) // 2�K�����I�[���� ��
            {
                for (int j = i - 1; j < (ord - 1) + (i - 1); ++j)
                    N_matrix[i * new_ncpnt + j] = Calc2DiffBsplineFunc(j, ord, knot[(ord - 1) + (i - 1)], &(const_cast<vector<double>&>(knot))[0]);
            }
            else // �I�[���� ��
            {
                // ��ԉE�̂�1
                N_matrix[i * new_ncpnt + (new_ncpnt - 1)] = 1;
            }
        }
    }

    // 3. �V��������_�����߂�
    {
        ctrlp->resize(new_ncpnt);
        vector<double> new_cps_X, new_cps_Y, new_cps_Z;

        // �A��������������
        new_cps_X = LUDecomposition(new_ncpnt, N_matrix, &P_array_x[0]);
        new_cps_Y = LUDecomposition(new_ncpnt, N_matrix, &P_array_y[0]);
        new_cps_Z = LUDecomposition(new_ncpnt, N_matrix, &P_array_z[0]);

        for (int i = 0; i < new_ncpnt; ++i)
            (*ctrlp)[i] = ControlPoint(new_cps_X[i], new_cps_Y[i], new_cps_Z[i]);
    }

    // ���
    delete N_matrix;
}

// 3�_���琬��|���S���̒P�ʉ��ςݖʖ@�����擾����
Vector3d CalcPolygonNormal(const Vector3d& v0, const Vector3d& v1, const Vector3d& v2)
{
    Vector3d vec01 = v1 - v0;
    Vector3d vec02 = v2 - v0;

    return (vec01 * vec02).Normalize();
}

// 2D:�w����W���S�Ɏw��rad��]������
void RotateCoord2DAroundCenter(double* const coord_2d, const double* const center, const double rad)
{
    // 2 �~ 2�̉�]�s��
    double rotate_mat[2 * 2] =
    {
        cos(rad), -sin(rad),
        sin(rad), cos(rad),
    };

    coord_2d[0] -= center[0];
    coord_2d[1] -= center[1];

    double temp[2];

    MatrixMultiply(2, 2, 1, rotate_mat, coord_2d, temp);

    // ���ʑ��
    coord_2d[0] = temp[0] + center[0];
    coord_2d[1] = temp[1] + center[1];
}

// 2D:���_���S�Ɏw��rad��]������
void RotateCoord2DAroundOrigin(double* const coord_2d, const double rad)
{
    double origin[2] = { 0, 0 }; // ���_
    RotateCoord2DAroundCenter(coord_2d, origin, rad);
}

// LU�����ŘA��������������
std::vector<double> LUDecomposition(const int size, const double* const aMatrix, const double* const b)
{
    int N = size; // �����z��̃T�C�Y

    // aMatrix�̒l�R�s�[���쐬
    double* aMatCopy = new double[N * N];
    for (int i = 0; i < N * N; i++)
        aMatCopy[i] = aMatrix[i];

    // L�s��(��s��ɏ�����)
    double **lMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        lMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            lMatrix[i][j] = 0;
    }

    // U�s��(�Ίp�v�f��1�ɏ�����)
    double **uMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        uMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            uMatrix[i][j] = (i == j) ? 1 : 0;
    }

    // �v�Z�p�̃o�b�t�@
    double **buffer = new double*[N];
    for (int i = 0; i < N; i++)
        buffer[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            buffer[i][j] = 0;
    }

    // LU�����J�n
    for (int i = 0; i < N; i++)
    {
        int n = N - i - 1;

        double l0 = lMatrix[i][i] = aMatCopy[0];

        // l1�������R�s�[
        double *l1;
        l1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatCopy[(j + 1) * N + 0];
        }

        // u1^double�������R�s�[
        double *u1;
        u1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatCopy[0 * N + (j + 1)] / l0;
        }

        // lu�����߂�
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                buffer[j][k] = l1[j] * u1[k];
            }
        }

        delete[] l1;
        delete[] u1;

        // A1�����߂�
        double **A1 = new double*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
                A1[j][k] = aMatCopy[(j + 1) * N + (k + 1)] - buffer[j][k];
        }

        // A1��V����aMatrix�Ƃ��ė��p����
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatCopy[i * N + j] = A1[i][j];
        }

        for (int i = 0; i < n; ++i)
            delete[] A1[i];
        delete[] A1;
    }

    // LU�s����g���ĘA��������������
    double *y = new double[N];
    for (int i = 0; i < N; i++)
    {
        double sum = 0;
        for (int k = 0; k <= i - 1; k++)
        {
            sum += lMatrix[i][k] * y[k];
        }
        y[i] = (b[i] - sum) / lMatrix[i][i];
    }

    std::vector<double> x; // ���x�N�g��
    x.resize(N);
    for (int i = N - 1; i >= 0; i--)
    {
        double sum = 0;
        for (int j = i + 1; j <= N - 1; j++)
        {
            sum += uMatrix[i][j] * x[j];
        }
        x[i] = y[i] - sum;
    }

    delete[] aMatCopy;
    for (int i = 0; i < N; ++i)
        delete[] (delete[] (delete[] lMatrix[i], uMatrix[i]), buffer[i]);
    delete[](delete[](delete[] lMatrix, uMatrix), buffer);
    delete[] y;

    return x;
}