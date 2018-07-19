#define _USE_MATH_DEFINES

#include "GV.h"
#include <math.h>
#include <ctime>
#include <fstream>
#include "Test.h"
#include "Model.h"
#include "Reader.h"
#include "Writer.h"
#include "Scene.h"
#include "glUtil.h"

static int current_size; // ���݂̃e�X�g�p�̃f�B�X�v���C���X�g��
static int* displayLists; // �f�B�X�v���C���X�g�ۊǗp�|�C���^
static bool isFirst = true;
static bool isRegistered = false;

Scene* test_scene;

// �e�X�g�Ƃ��������`��I�I�I�I�I

// �`�悵�����̂�����
static vector<function<void(void)>> TestRegisterDraw
{
    //DrawBsplineFunctions, // B�X�v���C�����֐��`��
    //DrawBsplineCurves, // B�X�v���C���Ȑ��`��
    //DrawCircle_CGS3, // Nurbs�Ȑ��ŉ~�`��
    //DrawSphere_CGS3, // Nurbs�Ȗʂŋ���`��
    //DrawCylinder_CGS3, // Nurbs�Ȗʂŉ~����`��
    //DrawApproxCurve_CGS4, // �ߎ��Ȑ���`��
    //DrawApproxSurface_CGS5, // �ߎ��Ȗʂ�`��
    DrawCurveNearest_CGS6, // �ŋߓ_��`��_�Ȑ�
    //DrawSurfaceNearest_CGS7, // �ŋߓ_��`��_�Ȗ�
    //DrawSplitCurve_CGS8, // �����Ȑ���`��
};

// �����Ȑ��`��
void DrawSplitCurve_CGS8()
{

}

// �Q�ƋȐ�����ŋߓ_�����߂ĕ`��
void DrawCurveNearest_CGS6()
{
    auto reader = std::make_unique<KjsReader>();

    // �ΏۋȐ�/�Ȗ�
    BsplineCurve* curve1 = (BsplineCurve *)reader->GetObjectFromFile("CGS_bspline_curve_1.kjs");
    // �Q�ƋȐ�
    BsplineCurve* curveC = (BsplineCurve *)reader->GetObjectFromFile("CGS_bspline_curve_C.kjs");
    
    //// �ΏۋȐ����Z�O�����g8���������_�Q
    //{
    //    vector<Vector3d> passPnts, segPassPnts;
    //    passPnts = curve1->GetPositionVectorsByKnots(8);
    //    segPassPnts = curve1->GetPositionVectorsByKnots();
    //    DrawPoints(segPassPnts, Color::pink, 10);
    //    DrawPoints(passPnts, Color::green, 10);
    //}

    // �Q�Ɠ_�Q���擾
    vector<Vector3d> ref_pnts;
    curveC->GetPositionVectors(ref_pnts, 20); // 20��������21�_�����

    // �ŋߓ_�擾
    vector<NearestPointInfoC> nearest_pnts;
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(curve1->GetNearestPointInfoFromRef(ref_pnts[i]));

    // �`��
    {
        glLineWidth(1.0);
        glBegin(GL_LINES);
        {
            // �Q�Ɠ_�ƍŋߓ_�����Ԑ�
            glColor4dv(Color::orange);
            for (int i = 0; i < (int)ref_pnts.size(); i++)
            {
                glVertex3d(ref_pnts[i]);
                glVertex3d(nearest_pnts[i].nearestPnt);
            }
        }
        glEnd();

        glPointSize(5.0);
        glBegin(GL_POINTS);
        {
            // �Q�Ɠ_
            glColor4dv(Color::red);
            for (int i = 0; i < (int)ref_pnts.size(); i++)
                glVertex3d(ref_pnts[i]);

            // �ŋߓ_
            glColor4dv(Color::light_green);
            for (int i = 0; i < (int)nearest_pnts.size(); i++)
                glVertex3d(nearest_pnts[i].nearestPnt);
        }
        glEnd();
    }

    // �ڍׂ�csv�f���o��
    ExportCsv_NearestInfoCurve("nearest_pnt_CGS6.csv", nearest_pnts, true);

    if (isFirst)
    {
        test_scene->AddObject(curve1);
        test_scene->AddObject(curveC);
    }
}

// �Q�ƋȖʂ���ŋߓ_�����߂ĕ`��
void DrawSurfaceNearest_CGS7()
{
    auto reader = std::make_unique<KjsReader>();

    // �ΏۋȐ�/�Ȗ�
    BsplineSurface* surf1 = (BsplineSurface *)reader->GetObjectFromFile("CGS_bspline_surface_1.kjs");
    // �Q�ƋȐ�
    Curve* curveS = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_S.kjs");

    // �Q�Ɠ_�Q(21�_)���擾
    vector<Vector3d> ref_pnts;
    curveS->GetPositionVectors(ref_pnts, 20);
    vector<NearestPointInfoS> nearest_pnts; // �ŋߓ_�Q

    clock_t start = clock();

    // �ŋߓ_�擾
    for (const auto& ref : ref_pnts)
        //nearest_pnts.push_back(surf1->GetNearestPointInfoFromRef(ref, Surface::Project));
        nearest_pnts.push_back(surf1->GetNearestPointInfoFromRef(ref, Surface::Isoline));

    clock_t end = clock();
    cout << "�g�������X : "  << EPS::NEAREST << "  �ŋߓ_�擾(21�_)���� = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

    vector<vector<Vector3d>> startPnts; // �J�n�_�Q
    int seg_split_u = 1;
    int seg_split_v = 1;
    surf1->GetPointsByKnots(startPnts, seg_split_u, seg_split_v);

    // �`��
    {
        glLineWidth(1.0);
        glBegin(GL_LINES);
        {
            // �Q�Ɠ_�ƍŋߓ_�����Ԑ���
            glColor4dv(Color::orange);
            for (int i = 0; i < (int)ref_pnts.size(); i++)
            {
                glVertex3d(ref_pnts[i]);
                glVertex3d(nearest_pnts[i].nearestPnt);
            }
        }
        glEnd();

        glPointSize(5.0);
        glBegin(GL_POINTS);
        {
            // �Q�Ɠ_
            glColor4dv(Color::red);
            for (int i = 0; i < (int)ref_pnts.size(); i++)
                glVertex3d(ref_pnts[i]);

            // �ŋߓ_
            glColor4dv(Color::light_green);
            for (int i = 0; i < (int)nearest_pnts.size(); i++)
                glVertex3d(nearest_pnts[i].nearestPnt);
        }
        glEnd();

        glPointSize(10.0);
        glBegin(GL_POINTS);
        {
            // �J�n�_
            glColor4dv(Color::pink);
            for (const auto& startRow : startPnts)
                for (const auto& start : startRow)
                    glVertex3d(start);
        }
        glEnd();
    }

    // �ڍׂ�csv�f���o��
    ExportCsv_NearestInfoSurface("nearest_pnt_CGS7.csv", nearest_pnts, true);

    if (isFirst)
    {
        test_scene->AddObject(surf1);
        test_scene->AddObject(curveS);
    }
}

// �ߎ����ċȖʂ�`��
void DrawApproxSurface_CGS5()
{
    auto reader = std::make_unique<KjsReader>();

    auto surf = (BsplineSurface *)reader->GetObjectFromFile("CGS_bspline_surface_1.kjs");

    // �ߎ��Ȗ�
    std::unique_ptr<Surface> surf_knot_remake, surf_knot_split_remake;

    // �Ȗʂ̋ߎ�(�m�b�g�ʒu�̂�)
    {
        vector<vector<Vector3d>> passPnts;
        surf->GetPointsByKnots(passPnts, 1, 1);
        for (const auto& pnts : passPnts)
            DrawPoints(pnts, Color::green, 10);

        surf_knot_remake = surf->GetSurfaceFromPoints(passPnts, Color::blue_alpha, 20);
    }
    // �Ȗʂ̋ߎ�(�m�b�g�ʒu�̂� + �Z�O�����g�ʒu3����)
    {
        vector<vector<Vector3d>> passPnts;
        surf->GetPointsByKnots(passPnts, 3, 3);
        for (const auto& pnts : passPnts)
            DrawPoints(pnts, Color::pink, 10);

        surf_knot_split_remake = surf->GetSurfaceFromPoints(passPnts, Color::orange_alpha, 20);
    }

    if (isFirst)
    {
        test_scene->AddObject(surf);
        //test_scene->AddObject(surf_knot_remake);
        test_scene->AddObject(surf_knot_split_remake.get());
    }
}

// �ߎ����ċȐ���`��
void DrawApproxCurve_CGS4()
{
    auto reader = std::make_unique<KjsReader>();

    BsplineCurve* curve1 = (BsplineCurve *)reader->GetObjectFromFile("CGS_bspline_curve_1.kjs");
    BsplineCurve* curve2 = (BsplineCurve *)reader->GetObjectFromFile("CGS_bspline_curve_2.kjs");

    // �ߎ��Ȑ�
    std::unique_ptr<Curve> curve1_remake, curve2_remake;
    std::unique_ptr<Curve> curve1_remake_split, curve2_remake_split;

    // �Ȑ�1�̋ߎ�(�m�b�g�ʒu�̂�)
    {
        vector<Vector3d> passPnts;
        passPnts = curve1->GetPositionVectorsByKnots();
        DrawPoints(passPnts, Color::green, 10);

        curve1_remake = curve1->GetCurveFromPoints(passPnts, Color::red, 3);
    }
    // �Ȑ�1�̋ߎ�(�m�b�g�ʒu�̂� + �Z�O�����g�ʒu3����)
    {
        vector<Vector3d> passPnts;
        passPnts = curve1->GetPositionVectorsByKnots(3);
        vector<Vector3d> passPntsOnlyKnot;
        passPntsOnlyKnot = curve1->GetPositionVectorsByKnots();
        DrawPoints(passPntsOnlyKnot, Color::green, 10);
        DrawPoints(passPnts, Color::pink, 10);

        curve1_remake_split = curve1->GetCurveFromPoints(passPnts, Color::orange, 3);
    }

    printf("�Ȑ�1�Ƌߎ��Ȑ��̑��዗������\n");
    printf("�m�b�g�ʒu�̂�:    %f\n", curve1->CalcDifferency(curve1_remake.get()));
    printf("�Z�O�����g�ʒu��:  %f\n", curve1->CalcDifferency(curve1_remake_split.get()));
    printf("\n");

    printf("�Ȑ�1�Ƌߎ��Ȑ��̑��዗������ Ver.2 \n");
    printf("�m�b�g�ʒu�̂�:    %f\n", curve1->CalcDifferency2(curve1_remake.get()));
    printf("�Z�O�����g�ʒu��:  %f\n", curve1->CalcDifferency2(curve1_remake_split.get()));
    printf("\n");

    printf("�Ȑ�1�Ƌߎ��Ȑ��̈�ԉ������鋗�� \n");
    printf("�m�b�g�ʒu�̂�:    %f\n", curve1->CalcFarthestDistant(curve1_remake.get()));
    printf("�Z�O�����g�ʒu��:  %f\n", curve1->CalcFarthestDistant(curve1_remake_split.get()));
    printf("\n");

    // �Ȑ�2�̋ߎ�(�m�b�g�ʒu�̂�)
    {
        vector<Vector3d> passPnts;
        passPnts = curve2->GetPositionVectorsByKnots();
        DrawPoints(passPnts, Color::green, 10);

        curve2_remake = curve2->GetCurveFromPoints(passPnts, Color::red, 3);
    }
    // �Ȑ�2�̋ߎ�(�m�b�g�ʒu�̂� + �Z�O�����g�ʒu3����)
    {
        vector<Vector3d> passPnts;
        passPnts = curve2->GetPositionVectorsByKnots(3);
        vector<Vector3d> passPntsOnlyKnot;
        passPntsOnlyKnot = curve2->GetPositionVectorsByKnots();
        DrawPoints(passPntsOnlyKnot, Color::green, 10);
        DrawPoints(passPnts, Color::pink, 10);

        curve2_remake_split = curve2->GetCurveFromPoints(passPnts, Color::orange, 3);
    }

    printf("�Ȑ�2�Ƌߎ��Ȑ��̑��዗������\n");
    printf("�m�b�g�ʒu�̂�:    %f\n", curve2->CalcDifferency(curve2_remake.get()));
    printf("�Z�O�����g�ʒu��:  %f\n", curve2->CalcDifferency(curve2_remake_split.get()));

    printf("�Ȑ�2�Ƌߎ��Ȑ��̑��዗������ Ver.2 \n");
    printf("�m�b�g�ʒu�̂�:    %f\n", curve2->CalcDifferency2(curve2_remake.get()));
    printf("�Z�O�����g�ʒu��:  %f\n", curve2->CalcDifferency2(curve2_remake_split.get()));

    printf("�Ȑ�2�Ƌߎ��Ȑ��̈�ԉ������鋗�� \n");
    printf("�m�b�g�ʒu�̂�:    %f\n", curve2->CalcFarthestDistant(curve2_remake.get()));
    printf("�Z�O�����g�ʒu��:  %f\n", curve2->CalcFarthestDistant(curve2_remake_split.get()));

    if (isFirst)
    {
        test_scene->AddObject(curve1);
        test_scene->AddObject(curve1_remake.get());
        test_scene->AddObject(curve1_remake_split.get());

        test_scene->AddObject(curve2);
        test_scene->AddObject(curve2_remake.get());
        test_scene->AddObject(curve2_remake_split.get());
    }
}

// Nurbs�Ȗʂŉ~����`��
void DrawCylinder_CGS3()
{
    // �悭�Ȃ����ǖ�����苁�߂�
    double cp1_p[2] = { 5, 5 * sqrt(3) };
    RotateCoord2DAroundOrigin(cp1_p, (double)2 / 3 * M_PI);

    ControlPoint cp0[9]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 5 * sqrt(3), 0, (double)1 / 2),
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),

        ControlPoint(5, 0, 10, 1.0),
        ControlPoint(5, 5 * sqrt(3), 10, (double)1 / 2),
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
    };
    ControlPoint cp1[9]
    {
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),
        ControlPoint(cp1_p[0], cp1_p[1], 0, (double)1 / 2),
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),

        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
        ControlPoint(cp1_p[0], cp1_p[1], 10, (double)1 / 2),
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
    };
    ControlPoint cp2[9]
    {
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),
        ControlPoint(5, -5 * sqrt(3), 0, (double)1 / 2),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
        ControlPoint(5, -5 * sqrt(3), 10, (double)1 / 2),
        ControlPoint(5, 0, 10, 1.0),
    };


    double knotU[6] = { 0, 0, 0, 1, 1, 1 };
    double knotV[4] = { 0, 0, 1, 1 };

    NurbsSurface* surf0 = new NurbsSurface(3, 2, cp0, 3, 2, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf1 = new NurbsSurface(3, 2, cp1, 3, 2, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf2 = new NurbsSurface(3, 2, cp2, 3, 2, knotU, knotV, Color::green_alpha);

    if (isFirst)
    {
        test_scene->AddObject(surf0);
        test_scene->AddObject(surf1);
        test_scene->AddObject(surf2);
    }
}

// Nurbs�Ȗʂŋ���`��
void DrawSphere_CGS3()
{
    ControlPoint cp0[9]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 0, -5, 1 / sqrt(2)),
        ControlPoint(0, 0, -5, 1.0),

        ControlPoint(5, 5, 0, 1 / sqrt(2)),
        ControlPoint(5, 5, -5, (double)1 / 2),
        ControlPoint(0, 5, -5, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp1[9]
    {
        ControlPoint(0, 0, 5, 1.0),
        ControlPoint(5, 0, 5, 1 / sqrt(2)),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(0, 5, 5, 1 / sqrt(2)),
        ControlPoint(5, 5, 5, (double)1 / 2),
        ControlPoint(5, 5, 0, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp2[9]
    {
        ControlPoint(0, 0, -5, 1.0),
        ControlPoint(-5, 0, -5, 1 / sqrt(2)),
        ControlPoint(-5, 0, 0, 1.0),

        ControlPoint(0, 5, -5, 1 / sqrt(2)),
        ControlPoint(-5, 5, -5, (double)1 / 2),
        ControlPoint(-5, 5, 0, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp3[9]
    {
        ControlPoint(-5, 0, 0, 1.0),
        ControlPoint(-5, 0, 5, 1 / sqrt(2)),
        ControlPoint(0, 0, 5, 1.0),

        ControlPoint(-5, 5, 0, 1 / sqrt(2)),
        ControlPoint(-5, 5, 5, (double)1 / 2),
        ControlPoint(0, 5, 5, 1 / sqrt(2)),

        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(0, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp4[9]
    {
        ControlPoint(0, 0, -5, 1.0),
        ControlPoint(5, 0, -5, 1 / sqrt(2)),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(0, -5, -5, 1 / sqrt(2)),
        ControlPoint(5, -5, -5, (double)1 / 2),
        ControlPoint(5, -5, 0, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp5[9]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 0, 5, 1 / sqrt(2)),
        ControlPoint(0, 0, 5, 1.0),

        ControlPoint(5, -5, 0, 1 / sqrt(2)),
        ControlPoint(5, -5, 5, (double)1 / 2),
        ControlPoint(0, -5, 5, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp6[9]
    {
        ControlPoint(-5, 0, 0, 1.0),
        ControlPoint(-5, 0, -5, 1 / sqrt(2)),
        ControlPoint(0, 0, -5, 1.0),

        ControlPoint(-5, -5, 0, 1 / sqrt(2)),
        ControlPoint(-5, -5, -5, (double)1 / 2),
        ControlPoint(0, -5, -5, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp7[9]
    {
        ControlPoint(0, 0, 5, 1.0),
        ControlPoint(-5, 0, 5, 1 / sqrt(2)),
        ControlPoint(-5, 0, 0, 1.0),

        ControlPoint(0, -5, 5, 1 / sqrt(2)),
        ControlPoint(-5, -5, 5, (double)1 / 2),
        ControlPoint(-5, -5, 0, 1 / sqrt(2)),

        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(0, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };

    double knotU[6] = { 0, 0, 0, 1, 1, 1 };
    double knotV[6] = { 0, 0, 0, 1, 1, 1 };

    NurbsSurface* surf0 = new NurbsSurface(3, 3, cp0, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf1 = new NurbsSurface(3, 3, cp1, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf2 = new NurbsSurface(3, 3, cp2, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf3 = new NurbsSurface(3, 3, cp3, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf4 = new NurbsSurface(3, 3, cp4, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf5 = new NurbsSurface(3, 3, cp5, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf6 = new NurbsSurface(3, 3, cp6, 3, 3, knotU, knotV, Color::green_alpha);
    NurbsSurface* surf7 = new NurbsSurface(3, 3, cp7, 3, 3, knotU, knotV, Color::green_alpha);

    if (isFirst)
    {
        test_scene->AddObject(surf0);
        test_scene->AddObject(surf1);
        test_scene->AddObject(surf2);
        test_scene->AddObject(surf3);
        test_scene->AddObject(surf4);
        test_scene->AddObject(surf5);
        test_scene->AddObject(surf6);
        test_scene->AddObject(surf7);
    }
}

// Nurbs�Ȑ��ŉ~��`��
void DrawCircle_CGS3()
{
    ControlPoint cp0[3]
    {
        ControlPoint(5, 0, 0, 1),
        ControlPoint(5, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1),
    };
    ControlPoint cp1[3]
    {
        ControlPoint(0, 5, 0, sqrt(2)),
        ControlPoint(-5, 5, 0, 1),
        ControlPoint(-5, 0, 0, sqrt(2)),
    };
    ControlPoint cp2[3]
    {
        ControlPoint(-5, 0, 0, sqrt(2)),
        ControlPoint(-5, -5, 0, 1),
        ControlPoint(0, -5, 0, sqrt(2)),
    };
    ControlPoint cp3[3]
    {
        ControlPoint(0, -5, 0, sqrt(2)),
        ControlPoint(5, -5, 0, 1),
        ControlPoint(5, 0, 0, sqrt(2)),
    };

    double knot[6] = { 0, 0, 0, 1, 1, 1 };

    NurbsCurve* curve0 = new NurbsCurve(3, cp0, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve1 = new NurbsCurve(3, cp1, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve2 = new NurbsCurve(3, cp2, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve3 = new NurbsCurve(3, cp3, 3, knot, Color::blue, 2.0);

    if (isFirst)
    {
        test_scene->AddObject(curve0);
        test_scene->AddObject(curve1);
        test_scene->AddObject(curve2);
        test_scene->AddObject(curve3);
    }
}

// ���֐�����т��̊֐����g����B�X�v���C���Ȑ���`�悷��
// ��1��ۑ�
static double knot_a[10] = { -3, -2, -1, 0, 1, 2, 3, 4, 5, 6 };
static double knot_b[10] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 3 };
static double knot_c[10] = { 0, 0, 0, 0, 1, 1, 3, 3, 3, 3 };
void DrawBsplineFunctions()
{
    // �ʂ̂Ƃ��Ńf�B�X�v���C���X�g��static�Ŏg���Ă�̂œ����ɂ͏����Ȃ�
    //DrawBsplineFunc(4, 6, 10, knot_a, -3.0, 6.0);
    DrawBsplineFunc(4, 6, 10, knot_b, 0.0, 3.0);
    //DrawBsplineFunc(4, 6, 10, knot_c, 0.0, 3.0);
}
void DrawBsplineCurves()
{
    static ControlPoint cp[6] =
    {
        ControlPoint(30, 80, 0),
        ControlPoint(50, 110, 0),
        ControlPoint(70, 120, 0),
        ControlPoint(90, 70, 0),
        ControlPoint(110, 60, 0),
        ControlPoint(130, 80, 0),
    };

    auto A = new BsplineCurve(4, cp, 6, knot_a, Color::blue, 1.0);
    auto B = new BsplineCurve(4, cp, 6, knot_b, Color::orange, 1.0);
    auto C = new BsplineCurve(4, cp, 6, knot_c, Color::green, 1.0);

    // ����_���݂ŕ`��
    A->DrawAsItIsWithCPs();
    B->DrawAsItIsWithCPs();
    C->DrawAsItIsWithCPs();
}

// �e�X�g�`��
void TestDraw()
{
    if (isRegistered)
    {
        // �S�f�B�X�v���C���X�g�R�[��
        for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
        {
            if (displayLists[i])
                glCallList(displayLists[i]);
        }

        // �e�X�g�V�[���`��
        test_scene->Draw();
    }
    else
    {
        Error::ShowMessage(
            "�e�X�g�p�f�B�X�v���C���X�g���쐬",
            "�����ō쐬���܂���, �K�؂ȏꏊ�ɓo�^�֐���u�����Ƃ𐄏����܂�.");

        TestRegister();
        glutPostRedisplay();
    }
}

// �f�B�X�v���C���X�g�ɕ`���o�^����(�`��O�Ɏ蓮�ŌĂяo�����Ƃ𐄏�)
void TestRegister()
{
    for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
    {
        displayLists = (int *)realloc(displayLists, sizeof(int) * ++current_size);
        displayLists[i] = glGenLists(1);

        glNewList(displayLists[i], GL_COMPILE);
        TestRegisterDraw[i]();
        glEndList();
    }

    isFirst = false;
    isRegistered = true;
}
