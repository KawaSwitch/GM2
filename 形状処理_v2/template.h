#pragma once

// 描画アシスト
template<class Anything> void DrawWithoutSetting(const Anything& obj)
{
    // デバグ描画用
    obj.Draw();
}

// LU分解して連立方程式を解く ジェネリック版
template <typename T>
vector<T> LUDecomposition(int size, T* aMatrix, T* b)
{
    int N = size; // 解く配列のサイズ

                  // L行列(零行列に初期化)
    T **lMatrix = new T*[N];
    for (int i = 0; i < N; i++)
        lMatrix[i] = new T[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            lMatrix[i][j] = 0;
    }

    // U行列(対角要素を1に初期化)
    T **uMatrix = new T*[N];
    for (int i = 0; i < N; i++)
        uMatrix[i] = new T[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            uMatrix[i][j] = (i == j) ? 1 : 0;
    }

    // 計算用のバッファ
    T **buffer = new T*[N];
    for (int i = 0; i < N; i++)
        buffer[i] = new T[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            buffer[i][j] = 0;
    }

    // LU分解開始
    for (int i = 0; i < N; i++)
    {
        int n = N - i - 1;

        T l0 = lMatrix[i][i] = aMatrix[0];

        // l1成分をコピー
        T *l1 = new T[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatrix[(j + 1) * N + 0];
        }

        // u1^T成分をコピー
        T *u1 = new T[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatrix[0 * N + (j + 1)] / l0;
        }

        // luを求める
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                buffer[j][k] = l1[j] * u1[k];
            }
        }

        // A1を求める
        T **A1 = new T*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new T[n];

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
                A1[j][k] = aMatrix[(j + 1) * N + (k + 1)] - buffer[j][k];
        }

        // A1を新しいaMatrixとして利用する
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatrix[i * N + j] = A1[i][j];
        }
    }

    // LU行列を使って連立方程式を解く
    T *y = new T[N];
    for (int i = 0; i < N; i++)
    {
        T sum = 0;
        for (int k = 0; k <= i - 1; k++)
        {
            sum += lMatrix[i][k] * y[k];
        }
        y[i] = (b[i] - sum) / lMatrix[i][i];
    }

    vector<T> x; // 解ベクトル
    x.resize(N);
    for (int i = N - 1; i >= 0; i--)
    {
        T sum = 0;
        for (int j = i + 1; j <= N - 1; j++)
        {
            sum += uMatrix[i][j] * x[j];
        }
        x[i] = y[i] - sum;
    }

    return x;
}

// LU分解でN元連立方程式を解く
template <int N>
vector<double> LUDecomposition(double aMatrix[N][N], double b[N])
{
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

        double l0 = lMatrix[i][i] = aMatrix[0][0];

        // l1成分をコピー
        double *l1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatrix[j + 1][0];
        }

        // u1^T成分をコピー
        double *u1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatrix[0][j + 1] / l0;
        }

        // luを求める
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                buffer[j][k] = l1[j] * u1[k];
            }
        }

        // A1を求める
        double **A1 = new double*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
                A1[j][k] = aMatrix[j + 1][k + 1] - buffer[j][k];
        }

        // A1を新しいaMatrixとして利用する
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatrix[i][j] = A1[i][j];
        }
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

    vector<double> x; // 解ベクトル
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

    return x;
}