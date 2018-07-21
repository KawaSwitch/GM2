#include "BsplineSurface.h"
#include "BsplineCurve.h"
#include <iterator>

BsplineSurface::BsplineSurface(
    const int u_mord, const int v_mord, const ControlPoint* const cp,
    const int u_cp_size, const int v_cp_size, const double* const u_knot, const double* const v_knot,
    const GLdouble* const color, const GLdouble resol)
{
    _ordU = u_mord;
    _ordV = v_mord;
    _ncpntU = u_cp_size;
    _ncpntV = v_cp_size;
    _nknotU = u_mord + u_cp_size;
    _nknotV = v_mord + v_cp_size;
    _min_draw_param_U = u_knot[_ordU - 1];  _max_draw_param_U = u_knot[_ncpntU];
    _min_draw_param_V = v_knot[_ordV - 1];  _max_draw_param_V = v_knot[_ncpntV];

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetKnotVector(u_knot, _nknotU, _knotU);
    SetKnotVector(v_knot, _nknotV, _knotV);
    SetColor(color);
    _resolution = resol;

    // VBOを使う
    _isUseVBO = true;
    // IBOを使う
    //_isUseIBO = true;
}

// ノットベクトル設定
void BsplineSurface::SetKnotVector(const double* const knot, int size, vector<double>& _knot)
{
    if (size <= 0)
        Error::ShowAndExit("ノットベクトル設定失敗", "knot-vector size must be over 0.");

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// 指定した端の曲線を取得する
std::unique_ptr<Curve> BsplineSurface::GetEdgeCurve(const SurfaceEdge edge) const
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordV : _ordU;
    vector<double> edge_knot = (edge == U_min || edge == U_max) ? _knotV : _knotU;

    return std::unique_ptr<Curve>(new BsplineCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), &edge_knot[0], Color::red, _mesh_width));
}

// 事前描画
void BsplineSurface::PreDraw() const
{   
    // 解像度
    int RES = (int)_resolution;

    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;

    // 描画範囲を予め設定
    int u_min = (int)(_knotU[_ordU - 1] * RES);
    int v_min = (int)(_knotV[_ordV - 1] * RES);
    int u_max = (int)(_knotU[_ncpntU] * RES);
    int v_max = (int)(_knotV[_ncpntV] * RES);
    int u_size = u_max - u_min + 1;
    int v_size = v_max - v_min + 1;

    pnt.resize(u_size);
    for (int i = 0; i < u_size; i++)
        pnt[i].resize(v_size);

    nor.resize(u_size);
    for (int i = 0; i < u_size; i++)
        nor[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
            nor[i - u_min][j - v_min] = GetNormalVector(u, v).Normalize();
        }
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // 三角ポリゴン表示
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            glBegin(GL_TRIANGLES);

            // 上三角
            glNormal3d(nor[i - u_min][j - v_min]);
            glVertex3d(pnt[i - u_min][j - v_min]);

            glNormal3d(nor[i + 1 - u_min][j - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j - v_min]);

            glNormal3d(nor[i - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i - u_min][j + 1 - v_min]);
            
            // 下三角
            glNormal3d(nor[i + 1 - u_min][j - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j - v_min]);
            
            glNormal3d(nor[i + 1 - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j + 1 - v_min]);

            glNormal3d(nor[i - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i - u_min][j + 1 - v_min]);

            glEnd();
        }
    };
}

// メッシュ描画
void BsplineSurface::DrawMeshInternal() const
{
    Vector3d pnt;

    // メッシュの色は3dvで渡した方が綺麗(α=0)
    glColor4dv(_color);
    glLineWidth(_mesh_width);

    // U方向
    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(u, v);
            glVertex3d(pnt);
        }

        glEnd();
    }
    // V方向
    for (int i = (int)(_knotV[_ordV - 1] * 100); i <= (int)(_knotV[_ncpntV] * 100); i += 10)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = (int)(_knotU[_ordU - 1] * 100); j <= (int)(_knotU[_ncpntU] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(v, u);
            glVertex3d(pnt);
        }

        glEnd();
    }
}

// 頂点バッファ(VBO)作成
void BsplineSurface::CreateVBO() const
{
    // 解像度
    int RES = (int)_resolution;

    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;
    vector<Vector3d> pnt_vbo;
    vector<Vector3d> nor_vbo;

    // 描画範囲を予め設定
    int u_min = (int)(_knotU[_ordU - 1] * RES);
    int v_min = (int)(_knotV[_ordV - 1] * RES);
    int u_max = (int)(_knotU[_ncpntU] * RES);
    int v_max = (int)(_knotV[_ncpntV] * RES);
    int u_size = u_max - u_min + 1;
    int v_size = v_max - v_min + 1;

    pnt.resize(u_size);
    for (int i = 0; i < u_size; i++)
        pnt[i].resize(v_size);

    nor.resize(u_size);
    for (int i = 0; i < u_size; i++)
        nor[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
            nor[i - u_min][j - v_min] = GetNormalVector(u, v).Normalize();
        }
    }

    // VBO用の頂点取得
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            // 上三角
            pnt_vbo.push_back(pnt[i - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            // 下三角
            pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i + 1 - u_min][j + 1 - v_min]);
            pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            // 法線
            nor_vbo.push_back(nor[i - u_min][j - v_min]);
            nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);

            nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            nor_vbo.push_back(nor[i + 1 - u_min][j + 1 - v_min]);
            nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);
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
void BsplineSurface::DrawVBO() const
{
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

// 頂点バッファ(IBO)作成
void BsplineSurface::CreateIBO() const
{
    // 解像度
    int RES = 15;

    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;
    vector<Vector3d> pnt_vbo;
    vector<Vector3d> nor_vbo;
    vector<GLushort> pnt_ibo;

    // 描画範囲を予め設定
    int u_min = (int)(_knotU[_ordU - 1] * RES);
    int v_min = (int)(_knotV[_ordV - 1] * RES);
    int u_max = (int)(_knotU[_ncpntU] * RES);
    int v_max = (int)(_knotV[_ncpntV] * RES);
    int u_size = u_max - u_min + 1;
    int v_size = v_max - v_min + 1;

    pnt.resize(u_size);
    for (int i = 0; i < u_size; i++)
        pnt[i].resize(v_size);

    nor.resize(u_size);
    for (int i = 0; i < u_size; i++)
        nor[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
            nor[i - u_min][j - v_min] = GetNormalVector(u, v).Normalize();
        }
    }

    // IBO用の頂点取得
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            //pnt_ibo.push

            //pnt_vbo.push_back(pnt[i - u_min][j - v_min]);
            //pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            //pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            //pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            //pnt_vbo.push_back(pnt[i + 1 - u_min][j + 1 - v_min]);
            //pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            //// 法線
            //nor_vbo.push_back(nor[i - u_min][j - v_min]);
            //nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            //nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);

            //nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            //nor_vbo.push_back(nor[i + 1 - u_min][j + 1 - v_min]);
            //nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);
        }
    }

    _nVertex_cache = (int)pnt_vbo.size();

    // VBOの設定
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // IBOの設定
    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glGenBuffers(1, &_vbo_nor);
    //glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &nor_vbo[0], GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// IBOで描画
void BsplineSurface::DrawIBO() const
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // 頂点
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_DOUBLE, 0, 0);

    //// 法線
    //glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    //glEnableClientState(GL_NORMAL_ARRAY);
    //glNormalPointer(GL_DOUBLE, 0, (void *)0);

    // 描画
    glDrawElements(GL_TRIANGLES, _nVertex_cache, GL_UNSIGNED_INT, (void *)0);

    // clean up
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// 接線ベクトル描画
void BsplineSurface::DrawFirstDiffVectorsInternal() const
{
    Vector3d pnt, diff;
    glLineWidth(2.0);

    glBegin(GL_LINES);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
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
void BsplineSurface::DrawSecondDiffVectorsInternal() const
{
    Vector3d pnt, diff;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
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
void BsplineSurface::DrawNormalVectorsInternal() const
{ 
    Vector3d pnt, normal;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
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
void BsplineSurface::DrawCurvatureVectorsInternal() const
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
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
Vector3d BsplineSurface::CalcVector(
    const double u, const double v,
    const function<double(const unsigned, const unsigned, const double, const double* const)> BasisFuncU,
    const function<double(const unsigned, const unsigned, const double, const double* const)> BasisFuncV) const
{
    Vector3d vector;

    // 基底関数配列(行列計算用)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // 基底関数配列へ各基底関数を代入
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = BasisFuncU(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = BasisFuncV(i, _ordV, v, &_knotV[0]);

    // ベクトル算出(行列計算)
    vector = CalcVectorWithBasisFunctions(&N_array_U[0], &N_array_V[0]);

    delete[](delete[] N_array_U, N_array_V);
    return vector;
}

// 位置ベクトル取得
Vector3d BsplineSurface::GetPositionVector(const double u, const double v) const
{
    // u:0-diff v:0-diff
    return CalcVector(u, v, CalcBsplineFunc, CalcBsplineFunc);
}

// 接線ベクトル取得
Vector3d BsplineSurface::GetFirstDiffVectorU(const double u, const double v) const
{
    // u:1-diff v:0-diff
    return CalcVector(u, v, Calc1DiffBsplineFunc, CalcBsplineFunc);
}
Vector3d BsplineSurface::GetFirstDiffVectorV(const double u, const double v) const
{
    // u:0-diff v:1-diff
    return CalcVector(u, v, CalcBsplineFunc, Calc1DiffBsplineFunc);
}

// 2階微分ベクトル取得
Vector3d BsplineSurface::GetSecondDiffVectorUU(const double u, const double v) const
{
    // u:2diff v:0-diff
    return CalcVector(u, v, Calc2DiffBsplineFunc, CalcBsplineFunc);
}
Vector3d BsplineSurface::GetSecondDiffVectorUV(const double u, const double v) const
{
    // u:1-diff v:1-diff
    return CalcVector(u, v, Calc1DiffBsplineFunc, Calc1DiffBsplineFunc);
}
Vector3d BsplineSurface::GetSecondDiffVectorVV(const double u, const double v) const
{
    // u:0-diff v:2-diff
    return CalcVector(u, v, CalcBsplineFunc, Calc2DiffBsplineFunc);
}

// ノットベクトルをもとにして点群を取得する(pnts[v][u]と並べる)
// splitSegCnt: セグメントを何分割するかの回数(デフォルトは1 = 分割しない)
// NOTE: Vector3dとPoint3dは継承関係にあり型変換可能であるが,
//           コンテナ使用時の派生要素の渡し方が複雑なためこの関数を残した
void BsplineSurface::GetPointsByKnots(vector<vector<Vector3d>>& pnts, const int splitSegCnt_U, const int splitSegCnt_V) const
{
    // 点群のクリア
    for (auto& pntRow : pnts)
        pntRow.clear();
    pnts.clear();

    for (auto& pntRow : pnts)
    {
        pntRow.clear();
        vector<Vector3d>(pntRow).swap(pntRow);
    }
    pnts.clear();
    vector<vector<Vector3d>>(pnts).swap(pnts);

    vector<vector<Point3dS>> pntsInfo;
    this->GetPointsInfoByKnots(pntsInfo, splitSegCnt_U, splitSegCnt_V);

    // 点群情報から座標情報だけを抜き取る
    for (int i = 0, r = (int)pntsInfo.size(); i < r; ++i)
    {
        vector<Vector3d> pntsRow;
        pntsRow.reserve(pntsInfo[i].size());

        for (int j = 0, c = (int)pntsInfo[i].size(); j < c; ++j)
            pntsRow.emplace_back(pntsInfo[i][j]);

        pnts.push_back(pntsRow);
    }
}
// ノットベクトルをもとにして点群情報を取得する(pnts[v][u]と並べる)
// splitSegCnt: セグメントを何分割するかの回数(デフォルトは1 = 分割しない)
void BsplineSurface::GetPointsInfoByKnots(vector<vector<Point3dS>>& pnts, const int splitSegCnt_U, const int splitSegCnt_V) const
{
    // 点群情報のクリア
    for (auto& pntRow : pnts)
        pntRow.clear();
    pnts.clear();

    double skip;
    vector<double> knotsU, knotsV; // 点群を取得するパラメータ

    // U方向パラメータ位置
    {
        for (size_t i = _ordU - 1, kn = _knotU.size(); i < kn - _ordU; ++i)
        {
            skip = (_knotU[i + 1] - _knotU[i]) / (double)splitSegCnt_U;

            for (int j = 0; j < splitSegCnt_U; ++j)
                knotsU.push_back(_knotU[i] + skip * j);
        }
        knotsU.push_back(_knotU[_knotU.size() - _ordU]);
    }
    // V方向パラメータ位置
    {
        for (size_t i = _ordV - 1, kn = _knotV.size(); i < kn - _ordV; ++i)
        {
            skip = (_knotV[i + 1] - _knotV[i]) / (double)splitSegCnt_V;

            for (int j = 0; j < splitSegCnt_V; ++j)
                knotsV.push_back(_knotV[i] + skip * j);
        }
        knotsV.push_back(_knotV[_knotV.size() - _ordV]);
    }

    // 位置ベクトルを求める
    for (size_t j = 0, kv_size = knotsV.size(); j < kv_size; ++j)
    {
        vector<Point3dS> p_vec;
        p_vec.reserve(knotsU.size());

        for (size_t i = 0, ku_size = knotsU.size(); i < ku_size; ++i)
            p_vec.emplace_back(Point3dS(GetPositionVector(knotsU[i], knotsV[j]), knotsU[i], knotsV[j]));

        pnts.push_back(p_vec);
    }
}

// 通過点から逆変換してBスプライン曲面を取得する(メンバ関数版)
std::unique_ptr<Surface> BsplineSurface::GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, const GLdouble resol) const
{
    return GetBsplineSurfaceFromPoints(pnts, 4, 4, color, resol);
}
// 通過点から逆変化してBスプライン曲面を取得する
std::unique_ptr<Surface> GetBsplineSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const int ordU, const int ordV, const GLdouble* const color, const GLdouble resol)
{
    int passPntsCntU, passPntsCntV; // 各方向の通過点数
    int new_ncpntU, new_ncpntV; // 制御点数
    //int new_nknotU, new_nknotV; // ノット列サイズ

    vector<double> knotU, knotV; // ノットベクトル
    vector<ControlPoint> cps; // 制御点

    // 各変数初期化
    if (IsPassingPntsSetCorrectly(pnts))
    {
        passPntsCntU = (int)pnts[0].size();
        passPntsCntV = (int)pnts.size();

        new_ncpntU = (passPntsCntU - 1) + (ordU - 1);
        new_ncpntV = (passPntsCntV - 1) + (ordV - 1);

        //new_nknotU = ordU + new_ncpntU;
        //new_nknotV = ordV + new_ncpntV;
    }
    else
        return nullptr;

    // ノットベクトルの決定
    CalcKnotVectorByPassingPnts(pnts, ordU, ordV, &knotU, &knotV);

    // 制御点の決定
    {
        vector<vector<ControlPoint>> temp_cpsU(passPntsCntV), temp_cpsV(new_ncpntU);
        cps.resize(new_ncpntU * new_ncpntV);

        // U方向から制御点群を算出
        for (int i = 0; i < passPntsCntV; ++i)
        {
            vector<Vector3d> pntsU(passPntsCntU);
            vector<ControlPoint> ctrlpU;

            for (int j = 0; j < passPntsCntU; ++j)
                pntsU[j] = pnts[i][j];

            CalcControlPointsByPassingPnts(pntsU, ordU, knotU, &ctrlpU);
            temp_cpsU[i] = ctrlpU;
        }

        // V方向から制御点群を算出
        for (int i = 0; i < new_ncpntU; ++i)
        {
            vector<Vector3d> pntsV(passPntsCntV);
            vector<ControlPoint> ctrlpV;

            for (int j = 0; j < passPntsCntV; ++j)
                pntsV[j] = temp_cpsU[j][i];

            // 注) 通過点はU方向からの制御点
            // ここで求めた制御点が曲面の制御点となる
            CalcControlPointsByPassingPnts(pntsV, ordV, knotV, &ctrlpV);
            temp_cpsV[i] = ctrlpV;
        }

        // 転置
        for (int i = 0; i < new_ncpntU; ++i)
        {
            for (int j = 0; j < new_ncpntV; ++j)
            {
                cps[j * new_ncpntU + i] = temp_cpsV[i][j];
            }
        }
    }

    return std::unique_ptr<Surface>(new BsplineSurface(ordU, ordV, &cps[0], new_ncpntU, new_ncpntV, &knotU[0], &knotV[0], color));
}

// 最近点取得
NearestPointInfoS BsplineSurface::GetNearestPointInfoFromRef(const Vector3d& ref, const NearestSearch search) const
{
    int seg_split_u, seg_split_v; // セグメント分割数
    vector<vector<Point3dS>> startPnts; // 開始点群

    if (search == Project)
    {
        // 近い開始点を与えても時間がかかるだけなので大雑把に
        seg_split_u = seg_split_v = 1;
        this->GetPointsInfoByKnots(startPnts, seg_split_u, seg_split_v);
    }
    else if (search == Isoline)
    {
        seg_split_u = seg_split_v = 8; // とりあえず
        this->GetPointsInfoByKnots(startPnts, seg_split_u, seg_split_v);
    }
    else
        throw;

    return Surface::GetNearestPointInfoInternal(ref, startPnts, search);
}
