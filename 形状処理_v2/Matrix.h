#pragma once

// P~Q Æ Q~R ÌsñÌÏðvZ·é
template <class T>
void MatrixMultiply(int P, int Q, int R, const T* xmat, const T* ymat, T* zmat)
{
    // ú»
    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j < R; j++)
        {
            zmat[i * R + j] = 0.0;
        }
    }

    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j < R; j++)
        {
            for (int k = 0; k < Q; k++)
                zmat[i * R + j] += xmat[i * Q + k] * ymat[k * R + j];
        }
    }
}

// (1sPñsñ)~(Ps1ñsñ) ÌvZ
template <class T>
T MatrixMultiply(int P, const T* xmat, const T* ymat)
{
    double dot = 0.0;

    for (int i = 0; i < P; i++)
        dot += xmat[i] * ymat[i];

    return dot;
}

// sñÌ]uðæ¾·é
template <class T>
void Transpose(int row, int col, const T* mat, T* res_mat)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            res_mat[j*row + i] = mat[i*col + j];
        }
    }
}