#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "Curve.h"

// 曲面インターフェース
class Surface : public Object
{
protected:

    int _ordU, _ordV; // 階数
    int _nVertex; // 頂点個数
    int _ncpntU, _ncpntV; // 制御点数
    vector<double> _ctrlpX; // 計算用
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;
    double _min_draw_param_U, _max_draw_param_U; // 描画範囲パラメータ
    double _min_draw_param_V, _max_draw_param_V;
    int _mesh_displayList = -1; // メッシュ用ディスプレイリスト
    double _mesh_width; // メッシュ線の幅

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

        _ctrlp.reserve(size); _ctrlpX.reserve(size);
        _ctrlpY.reserve(size); _ctrlpZ.reserve(size);

        for (int i = 0; i < size; i++)
        {
            _ctrlp.emplace_back(cp[i]);

            //// 転置なし
            //_ctrlpX.emplace_back(cp[i].X); 
            //_ctrlpY.emplace_back(cp[i].Y);
            //_ctrlpZ.emplace_back(cp[i].Z);
        }

        // 計算用に転置する(もっと見栄えがいいやり方が欲しい)
        for (int i = 0; i < _ncpntU; i++)
        {
            for (int j = 0; j < _ncpntV; j++)
            {
                _ctrlpX.push_back(_ctrlp[j * _ncpntU + i].X);
                _ctrlpY.push_back(_ctrlp[j * _ncpntU + i].Y);
                _ctrlpZ.push_back(_ctrlp[j * _ncpntU + i].Z);
            }
        }
    }

    // メッシュ表示
    virtual void DrawMeshInternal() { };

    void DrawMesh()
    {
        if (!isUseLight) // 光源処理なしの場合は見栄えのためメッシュ表示する
            DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); });
    }

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double u, double v) = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVectorU(double u, double v) = 0; // 接線ベクトル
    virtual Vector3d GetFirstDiffVectorV(double u, double v) = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) { return Vector3d(); }; // 2階微分ベクトル
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) { return Vector3d(); };

    // 法線ベクトル取得
    Vector3d GetNormalVector(double u, double v)
    {
        return (GetFirstDiffVectorU(u, v) * GetFirstDiffVectorV(u, v));
    }

    // 描画用曲率ベクトル取得
    Vector3d GetCurvatureVector(double u, double v)
    {
        // 主曲率取得
        double max_kappa, min_kappa;
        GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

        // 単位法線ベクトル
        Vector3d e = GetNormalVector(u, v).Normalize();

        // 絶対値が大きい方を返す
        return (fabs(max_kappa) > fabs(min_kappa)) ?
            (1 / max_kappa) * e :
            (1 / min_kappa) * e;
    }

private:

    // 主曲率を取得
    void GetPrincipalCurvatures(double u, double v, double* max_kappa, double* min_kappa)
    {
        Vector3d e = GetNormalVector(u, v).Normalize(); // 単位法線ベクトル

        // 各自必要な値を計算
        double L = e.Dot(GetSecondDiffVectorUU(u, v));
        double M = e.Dot(GetSecondDiffVectorUV(u, v));
        double N = e.Dot(GetSecondDiffVectorVV(u, v));
        double E = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorU(u, v));
        double F = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorV(u, v));
        double G = GetFirstDiffVectorV(u, v).Dot(GetFirstDiffVectorV(u, v));

        double A = E * G - F * F;
        double B = 2 * F * M - E * N - G * L;
        double C = L * N - M * M;

        // 主曲率κを計算する（Aκ^2 + Bκ + C = 0 の2解）
        double kappa1, kappa2;
        SolveQuadraticEquation(A, B, C, &kappa1, &kappa2);

        // 最大主曲率と最小主曲率を返す
        *max_kappa = (kappa1 > kappa2) ? kappa1 : kappa2;
        *min_kappa = (kappa1 < kappa2) ? kappa1 : kappa2;
    }

    // 平均曲率取得
    double GetMeanCurvature(double u, double v)
    {
        // 主曲率取得
        double max_kappa, min_kappa;
        GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

        return (max_kappa + min_kappa) / 2.0;
    }

    // ガウス曲率取得
    double GetGaussianCurvature(double u, double v)
    {
        // 主曲率取得
        double max_kappa, min_kappa;
        GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

        return max_kappa * min_kappa;
    }

public:

    // オブジェクト描画
    void Draw() override
    {
        // オブジェクト自身はデプス考慮無し(見栄えのため)
        glDepthMask(GL_FALSE);

        if (isUseLight)
            glEnable(GL_LIGHTING);

        if (_isUseVBO)
        {
            // VBO
            if (_vbo == 0)
            {
                CreateVBO(); // VBO作成
                glutPostRedisplay();
            }
            else
                DrawVBO(); // 描画
        }
        else
        {
            // ディスプレイリスト
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_LIGHTING);

        // メッシュはディスプレイリスト
        DrawMesh();
    }

    // 制御点描画
    void DrawCPsInternal() override
    {
        glColor3d(1.0, 0.0, 0.0); // 赤
        glPointSize(5.0);
        glLineWidth(1.0);

        // 点群
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // U方向線群
        for (int i = 0; i < _ncpntV; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntU; j++)
                glVertex3d(_ctrlp[i * _ncpntU + j].X, _ctrlp[i * _ncpntU + j].Y, _ctrlp[i * _ncpntU + j].Z);
            glEnd();
        }
        // V方向線群
        for (int i = 0; i < _ncpntU; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntV; j++)
                glVertex3d(_ctrlp[j * _ncpntU + i].X, _ctrlp[j * _ncpntU + i].Y, _ctrlp[j * _ncpntU + i].Z);
            glEnd();
        }
    }

    // 指定したパラメータのアイソ曲線を取得する
    //virtual Curve* GetIsoCurve(ParamUV direct, double param) = 0;

    // 参照点からの最近点を取得(アイソが取れないので今は曲面からはみ出ない範囲のみ可能)
    Vector3d GetNearestPointFromRef(Vector3d ref)
    {
        // 許容値
        const double EPS = 10e-6;

        // 初期パラメータ
        double u = (_min_draw_param_U + _max_draw_param_U) / 2.0;
        double v = (_min_draw_param_V + _max_draw_param_V) / 2.0;

        // 正射影法(DAP法)
        Vector3d p = GetPositionVector(u, v);
        Vector3d pu = GetFirstDiffVectorU(u, v);
        Vector3d pv = GetFirstDiffVectorV(u, v);

        double delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.7;
        double delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.7;

        while (fabs(delta_u) > EPS || fabs(delta_v) > EPS)
        {
            u += delta_u;
            v += delta_v;

            ///// u, v方向ともにはみ出る場合も片方の判定だけで十分
            //// u方向がはみ出る場合
            //if (u < 0.0 || u > 1.0)
            //{
            //    if (u < 0.0)
            //    {
            //        Curve* edge = GetIsoCurve(V, 0.0);
            //        return edge->GetNearestPointFromRef(ref);
            //    }
            //    else
            //    {
            //        Curve* edge = GetIsoCurve(V, 1.0);
            //        return edge->GetNearestPointFromRef(ref);
            //    }
            //}
            //// v方向がはみ出る場合
            //else if (v < 0.0 || v > 1.0)
            //{
            //    if (v < 0.0)
            //    {
            //        Curve* edge = GetIsoCurve(U, 0.0);
            //        return edge->GetNearestPointFromRef(ref);
            //    }
            //    else
            //    {
            //        Curve* edge = GetIsoCurve(U, 1.0);
            //        return  edge->GetNearestPointFromRef(ref);
            //    }
            //}

            p = GetPositionVector(u, v);
            pu = GetFirstDiffVectorU(u, v);
            pv = GetFirstDiffVectorV(u, v);

            delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.7;
            delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.7;
        }

        return GetPositionVector(u, v);
    }

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};