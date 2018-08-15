#define _USE_MATH_DEFINES

#include <numeric>
#include <vector>
#include "CalcUtil.h"
#include "Matrix.h"
#include "Error.h"
using std::vector;

// 度数法から弧度法に変換する
double ToRad(const double degree)
{
    constexpr double C = M_PI / 180;
    return degree * C;
}
// 弧度法から度数法に変換する
double ToDeg(const double radian)
{
    constexpr double C = 180 / M_PI;
    return radian * C;
}

// 平均値を取得する
double GetAverage(const vector<double>& array)
{
    return std::accumulate(array.begin(), array.end(), 0.0) / array.size();
}

// 2次方程式 ax^2 + bx + c = 0 の 2解x1,x2 を求める
// 虚数解の場合 x1 ± x2i を満たす x1,x2 を求める
void SolveQuadraticEquation(const double a, const double b, const double c, double* const x1, double* const x2)
{
    // 判別式
    double D = b * b - 4 * a * c;

    // 異なる2つの実数解
    if (D > 0)
    {
        *x1 = (-b + sqrt(D)) / (2 * a);
        *x2 = (-b - sqrt(D)) / (2 * a);
    }
    // 重解
    else if (D == 0)
    {
        // 重解のとき x = -(b / 2a)
        *x1 = *x2 = -b / (2 * a);
    }
    // 異なる2つの虚数解
    else
    {
        *x1 = -b / (2 * a); // real part
        *x2 = sqrt(-D) / (2 * a); // imaginary part
    }
}

// 二項係数を求める
constexpr double Binomial(const int n, const int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

// 階乗値を計算する
constexpr int Factorial(const int n)
{
    return (n == 0) ? 1 : n * Factorial(n - 1);
}

// バーンスタイン基底関数を求める
double CalcBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    return Binomial(N, i) * pow(t, i) * pow(1 - t, N - i);
}

// 1階微分用バーンスタイン基底関数を求める
double Calc1DiffBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    return Binomial(N, i) * (i * pow(t, i - 1) * pow(1 - t, N - i) - (N - i) * pow(t, i) * pow(1 - t, N - i - 1));
}

// 2階微分用バーンスタイン基底関数を求める
double Calc2DiffBernsteinFunc(const unsigned int i, const unsigned int N, const double t)
{
    double temp_left = i * ((i - 1) * pow(t, i - 2) * pow(1 - t, N - i)
        - pow(t, i - 1) * (N - i) * pow(1 - t, N - i - 1));

    double temp_right = (N - i) * (i * pow(t, i - 1) * pow(1 - t, N - i - 1)
        - pow(t, i) * (N - i - 1) * pow(1 - t, N - i - 2));

    return Binomial(N, i) * (temp_left - temp_right);
}

// Bスプライン基底関数値を求める
double CalcBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
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

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = ((t - knot[i]) / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = ((knot[i + M] - t) / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return temp_left + temp_right;
    }
}

// 1階微分用Bスプライン基底関数値を求める
double Calc1DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
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

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// 2階微分用Bスプライン基底関数値を求める
double Calc2DiffBsplineFunc(const unsigned int i, const unsigned int M, const double t, const double* const knot)
{
    if (M == 1)
    {
        // 端っこは特別扱い
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

        // 0ディバイドに気を付ける
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * Calc1DiffBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * Calc1DiffBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// 通過点が正しくセットされているか
// U方向/V方向の点の数がそれぞれ等しいかをチェックする
bool IsPassingPntsSetCorrectly(const vector<vector<Vector3d>>& pnts)
{
    if (pnts.size() < 1)
        return false;

    // 内側(u方向)だけ調べればよい
    for (const auto& u_pnts : pnts)
    {
        if (pnts[0].size() != u_pnts.size())
            return false;
    }

    // すべて同じ個数なら正しい(n×m個)
    return true;
}

// 通過点と階数からノットベクトルを算出する
void CalcKnotVectorByPassingPnts(const vector<Vector3d>& pnts, const int ord, vector<double>* knot)
{
    if (knot == nullptr)
    {
        Error::ShowAndExit("ヌルポインタ");
        return;
    }

    // ノット列サイズは制御点数と階数の和
    knot->resize((pnts.size() + ord - 2) + ord);

    vector<double> seg_dist; // セグメント間の距離
    seg_dist.resize(pnts.size() - 1);

    double minParam = 0.0;
    double maxParam = 1.0;
    double paramRange = fabs(maxParam - minParam);

    // 通過点間の距離の総和
    double sum = 0.0;
    for (size_t i = 0, n = pnts.size(); i < n - 1; ++i)
    {
        seg_dist[i] = pnts[i].DistanceFrom(pnts[i + 1]);
        sum += seg_dist[i];
    }

    // ノット間隔の割合は通過点間の距離に比例させる
    for (size_t i = 0, n = knot->size(); i < n; ++i)
    {
      if (i < (unsigned)ord) // 最初は階数分重ねる
            (*knot)[i] = minParam;
        else if (i < knot->size() - ord) // 距離に比例
        {
            (*knot)[i] = (*knot)[i - 1] + (seg_dist[i - ord] / sum) * paramRange;
        }
        else // 最後も階数分重ねる
            (*knot)[i] = maxParam;
    }
}

// 複数のノット列を正規化する
void NormalizeKnotVector(const vector<vector<double>>& knot_array, const int ord, vector<double>* knot_normal)
{
    if (knot_normal == nullptr)
    {
        Error::ShowAndExit("ヌルポインタ");
        return;
    }
    if (knot_array.size() < 1)
    {
        Error::ShowAndExit("空指定");
        return;
    }

    // 正規化前は0番目のノット列で初期化しておく
    knot_normal->resize(knot_array[0].size());
    for (size_t i = 0, n = knot_array[0].size(); i < n; ++i)
        (*knot_normal)[i] = knot_array[0][i];

    // 必要な個所(階数無視)を正規化する
    for (size_t col = ord, n = knot_array[0].size(); col < n - ord; ++col)
    {
        double sum = 0.0;

        for (size_t row = 0, n = knot_array.size(); row < n; ++row)
            sum += knot_array[row][col];

        // 平均
        (*knot_normal)[col] = sum / knot_array.size();
    }
}

// ノットベクトルを取得する
void CalcKnotVectorByPassingPnts(const vector<vector<Vector3d>>& pnts, const int ordU, const int ordV,
    vector<double>* knotU, vector<double>* knotV)
{
    // 各方向の通過点数
    int passPntsCntU, passPntsCntV;
    passPntsCntU = (int)pnts[0].size();
    passPntsCntV = (int)pnts.size();

    // 各方向の通過点から取得するノット列群
    vector<vector<double>> new_knotsU(passPntsCntV), new_knotsV(passPntsCntU);

    // U方向ノットベクトルの決定
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
        // 正規化
        NormalizeKnotVector(new_knotsU, ordU, knotU);
    }
    // V方向ノットベクトルの決定
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
        // 正規化
        NormalizeKnotVector(new_knotsV, ordV, knotV);
    }
}

// 1方向の点群からそれらを通る曲線の制御点を算出する
/*
通過点条件と終端条件から連立方程式を作成し解く
ただし, 通過点数n 制御点数qn(n + 階数 - 2) ノット列サイズkn とする

[通過点条件]
<端点条件>
P(k_0) = Q_0
P(k_kn-1) = Q_qn-1
<それ以外の通過点条件>
P(k_階数) = Q_1
P(k_階数+1) = Q_2
...
P(k_kn-階数-1) = Q_qn-2

[終端条件]
P''(k_階数-1) = 0
P''(k_kn-階数-2) = 0
*/
void CalcControlPointsByPassingPnts(const vector<Vector3d>& pnts, const int ord, const vector<double>& knot, vector<ControlPoint>* ctrlp)
{
    // 各方向の通過点数
    int passPntsCnt = (int)pnts.size();
    // 新しい制御点数
    int new_ncpnt = (passPntsCnt - 1) + (ord - 1);

    // 通過点ベクトル(連立方程式用)
    vector<Vector3d> P_array;
    vector<double> P_array_x, P_array_y, P_array_z; // 射影版

                                                    // 基底関数行列
    double* N_matrix;

    // 1. 連立方程式を解く用の通過点ベクトル 作成
    {
        P_array.push_back(pnts[0]);
        P_array.push_back(Vector3d::Zero());
        for (int i = 1; i < passPntsCnt - 1; ++i)
            P_array.push_back(pnts[i]);
        P_array.push_back(Vector3d::Zero());
        P_array.push_back(pnts[passPntsCnt - 1]);

        // 計算用に各要素に射影
        for (auto it = P_array.begin(), end = P_array.end(); it != end; ++it)
        {
            P_array_x.push_back((*it).X);
            P_array_y.push_back((*it).Y);
            P_array_z.push_back((*it).Z);
        }
    }

    // 2. 基底関数用行列 作成
    {
        N_matrix = new double[new_ncpnt * new_ncpnt];

        // 零行列に初期化
        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix[i * new_ncpnt + j] = 0;
        }

        for (int i = 0; i < new_ncpnt; i++)
        {
            if (i == 0) // 終端条件 前
            {
                // 一番左のみ1
                N_matrix[i * new_ncpnt + 0] = 1;
            }
            else if (i == 1) // 2階微分終端条件 前
            {
                for (int j = i - 1; j < ord - 1; ++j)
                    N_matrix[i * new_ncpnt + j] = Calc2DiffBsplineFunc(j, ord, knot[ord - 1], &(const_cast<vector<double>&>(knot))[0]);
            }
            else if (i > 1 && i < new_ncpnt - 2)
            {
                for (int j = i - 1; j < (ord - 1) + (i - 1); ++j)
                    N_matrix[i * new_ncpnt + j] = CalcBsplineFunc(j, ord, knot[(ord - 1) + (i - 1)], &(const_cast<vector<double>&>(knot))[0]);
            }
            else if (i == new_ncpnt - 2) // 2階微分終端条件 後
            {
                for (int j = i - 1; j < (ord - 1) + (i - 1); ++j)
                    N_matrix[i * new_ncpnt + j] = Calc2DiffBsplineFunc(j, ord, knot[(ord - 1) + (i - 1)], &(const_cast<vector<double>&>(knot))[0]);
            }
            else // 終端条件 後
            {
                // 一番右のみ1
                N_matrix[i * new_ncpnt + (new_ncpnt - 1)] = 1;
            }
        }
    }

    // 3. 新しい制御点を求める
    {
        ctrlp->resize(new_ncpnt);
        vector<double> new_cps_X, new_cps_Y, new_cps_Z;

        // 連立方程式を解く
        new_cps_X = LUDecomposition(new_ncpnt, N_matrix, &P_array_x[0]);
        new_cps_Y = LUDecomposition(new_ncpnt, N_matrix, &P_array_y[0]);
        new_cps_Z = LUDecomposition(new_ncpnt, N_matrix, &P_array_z[0]);

        for (int i = 0; i < new_ncpnt; ++i)
            (*ctrlp)[i] = ControlPoint(new_cps_X[i], new_cps_Y[i], new_cps_Z[i]);
    }

    // 解放
    delete N_matrix;
}

// ノット列にノットパラメータを追加する
void CalcKnotsForAddingKnot(const double t, const vector<double>& knot, unsigned& insert, vector<double>& new_knot)
{
  if (new_knot.size() > 0)
    {
      new_knot.clear();
      new_knot.shrink_to_fit();
    }
  
  // 挿入先の決定
  for (unsigned i = 0, s = knot.size(); i < s - 1; ++i)
    {
      if (knot[i] <= t && t < knot[i+1])
	insert = i;
    }
    
  // 新しいノットベクトルを作成
  for (unsigned i = 0; i <= insert; ++i) new_knot.push_back(knot[i]);
  new_knot.push_back(t);
  for (unsigned i = insert+1; i < knot.size(); ++i) new_knot.push_back(knot[i]);
}

// ノット追加に伴う制御点の変更を計算する
void CalcControlPointsForAddingKnot(const double t, const int insert, const int ord, const vector<double>& knot, const vector<ControlPoint>& ctrlp, vector<ControlPoint>& new_ctrlp)
{
  Vector3d Q;
  int ncpnt = ctrlp.size();
  
  if (new_ctrlp.size() > 0)
    {
      new_ctrlp.clear();
      new_ctrlp.shrink_to_fit();
    }

  // 新制御点算出用の比率変数
  auto alpha = [&](int i, int j) -> double
    {
      if (j <= i - ord + 1)
	return 1;
      else if (i-ord+2 <= j && j <= i)
	return (t - knot[j]) / (knot[j+ord] - knot[j]);
      else
	return 0;
    };
  
  // 新しい制御点を作成
  new_ctrlp.push_back(ctrlp[0]);
  for (int j = 1; j < ncpnt; ++j)
    {
      Q = alpha(insert, j)*ctrlp[j] + (1-alpha(insert, j))*ctrlp[j-1];
      new_ctrlp.push_back(ControlPoint(Q.X, Q.Y, Q.Z, 1.0));
    }
  new_ctrlp.push_back(ctrlp[ncpnt-1]);
}



// 3点から成るポリゴンの単位化済み面法線を取得する
Vector3d CalcPolygonNormal(const Vector3d& v0, const Vector3d& v1, const Vector3d& v2)
{
    Vector3d vec01 = v1 - v0;
    Vector3d vec02 = v2 - v0;

    return (vec01 * vec02).Normalize();
}

// 2D:指定座標中心に指定rad回転させる
void RotateCoord2DAroundCenter(double* const coord_2d, const double* const center, const double rad)
{
    // 2 × 2の回転行列
    double rotate_mat[2 * 2] =
    {
        cos(rad), -sin(rad),
        sin(rad), cos(rad),
    };

    coord_2d[0] -= center[0];
    coord_2d[1] -= center[1];

    double temp[2];

    MatrixMultiply(2, 2, 1, rotate_mat, coord_2d, temp);

    // 結果代入
    coord_2d[0] = temp[0] + center[0];
    coord_2d[1] = temp[1] + center[1];
}

// 2D:原点中心に指定rad回転させる
void RotateCoord2DAroundOrigin(double* const coord_2d, const double rad)
{
    double origin[2] = { 0, 0 }; // 原点
    RotateCoord2DAroundCenter(coord_2d, origin, rad);
}

// LU分解で連立方程式を解く
std::vector<double> LUDecomposition(const int size, const double* const aMatrix, const double* const b)
{
    int N = size; // 解く配列のサイズ

    // aMatrixの値コピーを作成
    double* aMatCopy = new double[N * N];
    for (int i = 0; i < N * N; i++)
        aMatCopy[i] = aMatrix[i];

    // L行列(零行列に初期化)
    double **lMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        lMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            lMatrix[i][j] = 0;
    }

    // U行列(対角要素を1に初期化)
    double **uMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        uMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            uMatrix[i][j] = (i == j) ? 1 : 0;
    }

    // 計算用のバッファ
    double **buffer = new double*[N];
    for (int i = 0; i < N; i++)
        buffer[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            buffer[i][j] = 0;
    }

    // LU分解開始
    for (int i = 0; i < N; i++)
    {
        int n = N - i - 1;

        double l0 = lMatrix[i][i] = aMatCopy[0];

        // l1成分をコピー
        double *l1;
        l1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatCopy[(j + 1) * N + 0];
        }

        // u1^double成分をコピー
        double *u1;
        u1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatCopy[0 * N + (j + 1)] / l0;
        }

        // luを求める
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                buffer[j][k] = l1[j] * u1[k];
            }
        }

        delete[] l1;
        delete[] u1;

        // A1を求める
        double **A1 = new double*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
                A1[j][k] = aMatCopy[(j + 1) * N + (k + 1)] - buffer[j][k];
        }

        // A1を新しいaMatrixとして利用する
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatCopy[i * N + j] = A1[i][j];
        }

        for (int i = 0; i < n; ++i)
            delete[] A1[i];
        delete[] A1;
    }

    // LU行列を使って連立方程式を解く
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

    std::vector<double> x; // 解ベクトル
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
