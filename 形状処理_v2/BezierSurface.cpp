#include "BezierSurface.h"

BezierSurface::BezierSurface(
    int u_mord, int v_mord, ControlPoint* cp,
    int u_cp_size, int v_cp_size, GLdouble* color, GLdouble width)
{
    _ordU = u_mord; _ordV = v_mord;
    _ncpntU = u_cp_size; _ncpntV = v_cp_size;

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetColor(color);
    _mesh_width = width;

    // VBOを使う
    _isUseVBO = true;
}

// 事前描画
void BezierSurface::PreDraw()
{
    vector<vector<Vector3d>> pnt;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
        }
    }

    glColor4dv(_color);

    // 三角ポリゴン表示
    for (int i = 0; i <= 99; i += 1)
    {
        for (int j = 0; j <= 99; j += 1)
        {
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3d(pnt[i][j]);
            glVertex3d(pnt[i + 1][j]);
            glVertex3d(pnt[i][j + 1]);
            glVertex3d(pnt[i + 1][j + 1]);
            glEnd();
        }
    }
}

// メッシュ描画
void BezierSurface::DrawMeshInternal()
{
    Vector3d pnt;

    // メッシュの色は3dvで渡した方が綺麗(α=0)
    glColor3dv(_color);
    glLineWidth(_mesh_width);

    // U方向
    for (int i = 0; i <= 100; i += 5)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(u, v);
            glVertex3d(pnt);
        }

        glEnd();
    }
    // V方向
    for (int i = 0; i <= 100; i += 5)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(v, u);
            glVertex3d(pnt);
        }

        glEnd();
    }
}

// 頂点バッファ作成
void BezierSurface::CreateVBO()
{
    vector<vector<Vector3d>> pnt;
    vector<Vector3d> pnt_vbo;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
        }
    }

    // VBO用の頂点取得
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            pnt_vbo.push_back(pnt[i][j]);
            pnt_vbo.push_back(pnt[i + 1][j]);
            pnt_vbo.push_back(pnt[i + 1][j + 1]);

            pnt_vbo.push_back(pnt[i][j]);
            pnt_vbo.push_back(pnt[i][j + 1]);
            pnt_vbo.push_back(pnt[i + 1][j + 1]);
        }
    }

    _nVertex = (int)pnt_vbo.size();

    // VBOの設定
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VBOで描画
void BezierSurface::DrawVBO()
{
    glColor4dv(_color);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 接線ベクトル描画
void BezierSurface::DrawFirstDiffVectorsInternal()
{
    Vector3d pnt, diff;
    glLineWidth(2.0);

    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            // U方向
            glColor3dv(Color::red); // 赤
            pnt = GetPositionVector(u, v);
            diff = GetFirstDiffVectorU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // V方向
            glColor3dv(Color::green); // 緑
            diff = GetFirstDiffVectorV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 2階微分ベクトル描画
void BezierSurface::DrawSecondDiffVectorsInternal()
{
    Vector3d pnt, diff;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            // UU微分
            glColor3dv(Color::blue); // 青
            pnt = GetPositionVector(u, v);
            diff = GetSecondDiffVectorUU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // UV微分
            glColor3dv(Color::blue); // 青
            diff = GetSecondDiffVectorUV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // VV微分
            glColor3dv(Color::blue); // 青
            diff = GetSecondDiffVectorVV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 法線ベクトル描画
void BezierSurface::DrawNormalVectorsInternal()
{
    Vector3d pnt, normal;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            // 法線
            glColor3dv(Color::blue); // 青
            pnt = GetPositionVector(u, v);
            normal = GetNormalVector(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + normal);
        }
    }

    glEnd();
}

// 位置ベクトル取得
Vector3d BezierSurface::GetPositionVector(double u, double v)
{
    Vector3d pnt;
    double temp[100]; // 計算用

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBernsteinFunc(i, _ordV - 1, v);

    // 位置ベクトル算出(行列計算)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    pnt.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    pnt.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    pnt.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return pnt;
}

// 接線ベクトル取得
Vector3d BezierSurface::GetFirstDiffVectorU(double u, double v)
{
    Vector3d diff;
    double temp[100]; // 計算用

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc1DiffBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBernsteinFunc(i, _ordV - 1, v);

    // 位置ベクトル算出(行列計算)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BezierSurface::GetFirstDiffVectorV(double u, double v)
{
    Vector3d diff;
    double temp[100]; // 計算用

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc1DiffBernsteinFunc(i, _ordV - 1, v);

    // 位置ベクトル算出(行列計算)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}

// 2階微分ベクトル取得
Vector3d BezierSurface::GetSecondDiffVectorUU(double u, double v)
{
    Vector3d diff;
    double temp[100]; // 計算用

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc2DiffBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBernsteinFunc(i, _ordV - 1, v);

    // 位置ベクトル算出(行列計算)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BezierSurface::GetSecondDiffVectorUV(double u, double v)
{
    Vector3d diff;
    double temp[100]; // 計算用

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc1DiffBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc1DiffBernsteinFunc(i, _ordV - 1, v);

    // 位置ベクトル算出(行列計算)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BezierSurface::GetSecondDiffVectorVV(double u, double v)
{
    Vector3d diff;
    double temp[100]; // 計算用

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc2DiffBernsteinFunc(i, _ordV - 1, v);

    // 位置ベクトル算出(行列計算)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}