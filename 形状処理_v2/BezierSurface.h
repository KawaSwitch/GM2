#pragma once

#include "Surface.h"
#include "ControlPoint.h"

class BezierSurface : public Surface
{
private:

    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    //void DrawCurvatureVectorsInternal() override;
    void DrawMeshInternal() override;

    // Ž–‘O•`‰æ
    void PreDraw() override;
    void CreateVBO() override;
    void DrawVBO() override;

public:

    BezierSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, GLdouble* color, GLdouble width);

    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};