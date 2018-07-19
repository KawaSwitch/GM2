#include "BezierSurface.h"
#include "BezierCurve.h"

BezierSurface::BezierSurface(
    const int u_mord, const int v_mord, const ControlPoint* const cp,
    const int u_cp_size, const int v_cp_size, const GLdouble* const color, const GLdouble resol)
{
    _ordU = u_mord; _ordV = v_mord;
    _ncpntU = u_cp_size; _ncpntV = v_cp_size;
    _min_draw_param_U = 0.0;  _max_draw_param_U = 1.0;
    _min_draw_param_V = 0.0;  _max_draw_param_V = 1.0;

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetColor(color);
    _resolution = resol;

    // VBOを使う
    _isUseVBO = true;
}

// 指定した端の曲線を取得する
std::unique_ptr<Curve> BezierSurface::GetEdgeCurve(const SurfaceEdge edge) const
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordU : _ordV;

    return std::unique_ptr<Curve>(new BezierCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), _color, _mesh_width));
}

// 事前描画
void BezierSurface::PreDraw() const
{
    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    nor.resize(101);
    for (int i = 0; i < 101; i++)
        nor[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
            nor[i][j] = GetNormalVector(u, v).Normalize();
        }
    }

    //glColor4dv(_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // 三角ポリゴン表示
    for (int i = 0; i <= 99; i += 1)
    {
        for (int j = 0; j <= 99; j += 1)
        {
            glBegin(GL_TRIANGLE_STRIP);

            glNormal3d(nor[i][j]);
            glVertex3d(pnt[i][j]);

            glNormal3d(nor[i + 1][j]);
            glVertex3d(pnt[i + 1][j]);

            glNormal3d(nor[i][j + 1]);
            glVertex3d(pnt[i][j + 1]);
            
            glNormal3d(nor[i + 1][j + 1]);
            glVertex3d(pnt[i + 1][j + 1]);
            
            glEnd();
        }
    }
}

// メッシュ描画
void BezierSurface::DrawMeshInternal() const
{
    Vector3d pnt;

    // メッシュの色は3dvで渡した方が綺麗(α=0)
    glColor4dv(_color);
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
void BezierSurface::CreateVBO() const
{
    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;
    vector<Vector3d> pnt_vbo;
    vector<Vector3d> nor_vbo;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    nor.resize(101);
    for (int i = 0; i < 101; i++)
        nor[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
            nor[i][j] = GetNormalVector(u, v).Normalize();
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

            // 法線
            nor_vbo.push_back(nor[i][j]);
            nor_vbo.push_back(nor[i + 1][j]);
            nor_vbo.push_back(nor[i + 1][j + 1]);

            nor_vbo.push_back(nor[i][j]);
            nor_vbo.push_back(nor[i][j + 1]);
            nor_vbo.push_back(nor[i + 1][j + 1]);
        }
    }

    _nVertex_cache = (int)pnt_vbo.size();

    // VBOの設定
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_vbo_nor);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &nor_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VBOで描画
void BezierSurface::DrawVBO() const
{
    //glColor4dv(_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // 頂点
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    // 法線
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, (void *)0);

    // 描画
    glDrawArrays(GL_TRIANGLES, 0, _nVertex_cache);

    // clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 接線ベクトル描画
void BezierSurface::DrawFirstDiffVectorsInternal() const
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
            glColor4dv(Color::red); // 赤
            pnt = GetPositionVector(u, v);
            diff = GetFirstDiffVectorU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // V方向
            glColor4dv(Color::green); // 緑
            diff = GetFirstDiffVectorV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 2階微分ベクトル描画
void BezierSurface::DrawSecondDiffVectorsInternal() const
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
            glColor3d(1.0, 0.2, 0.2); // 赤
            pnt = GetPositionVector(u, v);
            diff = GetSecondDiffVectorUU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // UV微分
            glColor3d(0.2, 1.0, 0.2); // 緑
            diff = GetSecondDiffVectorUV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // VV微分
            glColor3d(0.2, 0.2, 1.0); // 青
            diff = GetSecondDiffVectorVV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 法線ベクトル描画
void BezierSurface::DrawNormalVectorsInternal() const
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
            glColor4dv(Color::blue); // 青
            pnt = GetPositionVector(u, v);
            normal = GetNormalVector(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + normal);
        }
    }

    glEnd();
}

// 曲率半径描画
void BezierSurface::DrawCurvatureVectorsInternal() const
{
    Vector3d pnt, curv;
    
    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = 0; i <= 100; i += 10)
    {
        for (int j = 0; j <= 100; j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(u, v);
            curv = GetCurvatureVector(u, v);

            // 曲率半径
            glColor4dv(Color::pink); // ピンク
            glBegin(GL_LINES);
            glVertex3d(pnt);
            glVertex3d(pnt + curv);
            glEnd();

            // 曲率中心
            glColor4dv(Color::light_green); // 黄緑
            glBegin(GL_POINTS);
            glVertex3d(pnt + curv);
            glEnd();
        }
    }
}

// 指定パラメータのベクトルを基底関数から算出する
Vector3d BezierSurface::CalcVector(
    const double u, const double v,
    const function<double(const unsigned, const unsigned, const double)> BasisFuncU,
    const function<double(const unsigned, const unsigned, const double)> BasisFuncV) const
{
    Vector3d vector;

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = BasisFuncU(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = BasisFuncV(i, _ordV - 1, v);

    // ベクトル算出(行列計算)
    vector = CalcVectorWithBasisFunctions(N_array_U, N_array_V);

    delete[] N_array_U, N_array_V;
    return vector;
}

// 位置ベクトル取得
Vector3d BezierSurface::GetPositionVector(const double u, const double v) const
{
    // u:0-diff v:0-diff
    return CalcVector(u, v, CalcBernsteinFunc, CalcBernsteinFunc);
}

// 接線ベクトル取得
Vector3d BezierSurface::GetFirstDiffVectorU(const double u, const double v) const
{
    // u:1-diff v:0-diff
    return CalcVector(u, v, Calc1DiffBernsteinFunc, CalcBernsteinFunc);
}
Vector3d BezierSurface::GetFirstDiffVectorV(const double u, const double v) const
{
    // u:0-diff v:1-diff
    return CalcVector(u, v, CalcBernsteinFunc, Calc1DiffBernsteinFunc);
}

// 2階微分ベクトル取得
Vector3d BezierSurface::GetSecondDiffVectorUU(const double u, const double v) const
{
    // u:2diff v:0-diff
    return CalcVector(u, v, Calc2DiffBernsteinFunc, CalcBernsteinFunc);
}
Vector3d BezierSurface::GetSecondDiffVectorUV(const double u, const double v) const
{
    // u:1-diff v:1-diff
    return CalcVector(u, v, Calc1DiffBernsteinFunc, Calc1DiffBernsteinFunc);
}
Vector3d BezierSurface::GetSecondDiffVectorVV(const double u, const double v) const
{
    // u:0-diff v:2-diff
    return CalcVector(u, v, CalcBernsteinFunc, Calc2DiffBernsteinFunc);
}

// 逆変換
std::unique_ptr<Surface> BezierSurface::GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, const GLdouble resol) const
{
    // 未実装
    return nullptr;
}

// 最近点取得
NearestPointInfoS BezierSurface::GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search) const
{
    // 未実装
    return NearestPointInfoS(Vector3d(), ref, 0, 0);
}