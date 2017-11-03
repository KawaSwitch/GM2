#pragma once

#include "Object.h"
#include "ControlPoint.h"

// 曲面インターフェース
class Surface : public Object
{
protected:

    int _ordU, _ordV; // 階数
    int _ncpntU, _ncpntV; // 制御点数
    vector<ControlPoint> _ctrlp; // 制御点

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

    // ベクトル取得関数
    virtual Vector3d GetPositionVector(double u, double v) { return Vector3d(); }; // 位置ベクトル
    virtual Vector3d GetFirstDiffVector(double u, double v) { return Vector3d(); }; // 接線ベクトル
    virtual Vector3d GetSecondDiffVector(double u, double v) { return Vector3d(); }; // 2階微分ベクトル
    virtual Vector3d GetCurvatureVector(double u, double v) { return Vector3d(); }; // 曲率ベクトル

public:

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

    ~Surface() { }
};