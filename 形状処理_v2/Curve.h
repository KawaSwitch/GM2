#pragma once

#include "GV.h"
#include "ControlPoint.h"

// 曲線インターフェース
class Curve : public Object
{
protected:

    int _ord; // 階数
    int _ncpnt; // 制御点数
    vector<ControlPoint> _ctrlp; // 制御点
    int _ctrlp_displayList = -1; // 制御点用ディスプレイリスト

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

    // 制御点線描画
    virtual void DrawControlPointsAndLines() override
    {
        if (_isDrawCtrlp)
        {
            if (_ctrlp_displayList == -1)
            {
                if (!(_ctrlp_displayList = glGenLists(1)))
                    return;

                // 登録
                glNewList(_ctrlp_displayList, GL_COMPILE);
                DrawCPsInternal();
                glEndList();

                glutPostRedisplay();
            }
            else
                glCallList(_ctrlp_displayList);
        }
    }

    virtual Vector3d GetPositionVector(double t) = 0; // 位置ベクトル

private:

    void DrawCPsInternal()
    {
        glColor3d(1.0, 0.0, 0.0); // 赤
        glPointSize(5.0);
        glLineWidth(1.0);

        // 点群
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // 線群
        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();
    }

public:

    virtual ~Curve() { };
};