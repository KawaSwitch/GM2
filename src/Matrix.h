#pragma once

#include "Point3d.h"

// P�~Q �� Q�~R �̍s��̐ς��v�Z����
template <class T>
void MatrixMultiply(int P, int Q, int R, const T* const xmat, const T* const ymat, T* const zmat)
{
    // ������
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

// (1�sP��s��)�~(P�s1��s��) �̌v�Z
template <class T>
T MatrixMultiply(int P, const T* const xmat, const T* const ymat)
{
    double dot = 0.0;

    for (int i = 0; i < P; i++)
        dot += xmat[i] * ymat[i];

    return dot;
}

// �s��̓]�u���擾����
template <class T>
void Transpose(int row, int col, const T* const mat, T* const res_mat)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            res_mat[j*row + i] = mat[i*col + j];
        }
    }
}
