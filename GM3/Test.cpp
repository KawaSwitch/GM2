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
#include "Intersection.h"
#include "MakeUV.h"

static int current_size;  // 現在のテスト用のディスプレイリスト数
static int *displayLists; // ディスプレイリスト保管用ポインタ
static bool isFirst = true;
static bool isRegistered = false;

Scene *test_scene;

// 形状の名称(ファイル名) TODO: どっかで共有
static const std::string bezierCurveB_name("CGS_bezier_curve_B.kjs");
static const std::string bezierSurfA_name("CGS_bezier_surface_A.kjs");
static const std::string bezierSurfD_name("CGS_bezier_surface_D.kjs");
static const std::string curve1_name("CGS_bspline_curve_1.kjs");
static const std::string curve2_name("CGS_bspline_curve_2.kjs");
static const std::string curveC_name("CGS_bspline_curve_C.kjs");
static const std::string curveS_name("CGS_bspline_curve_S.kjs");
static const std::string surf1_name("CGS_bspline_surface_1.kjs");
static const std::string curveG_name("CGS_bspline_curve_G.kjs");
static const std::string curve9_1_name("CGS_bspline_curve_9_1.kjs");
static const std::string curve9_2_name("CGS_bspline_curve_9_2.kjs");
static const std::string curve9_3_name("CGS_bspline_curve_9_3.kjs");
static const std::string curve9_4_name("CGS_bspline_curve_9_4.kjs");


// テストというか仮描画！！！！！

// 描画したいのをおく
static vector<function<void(void)>> TestRegisterDraw{
    //DrawBsplineFunctions, // Bスプライン基底関数描画
    //DrawBsplineCurves, // Bスプライン曲線描画
    //DrawCircle_CGS3, // Nurbs曲線で円描く
    //DrawSphere_CGS3, // Nurbs曲面で球を描く
    //DrawCylinder_CGS3, // Nurbs曲面で円柱を描く
    //DrawApproxCurve_CGS4, // 近似曲線を描画
    //DrawApproxSurface_CGS5, // 近似曲面を描画
    //DrawCurveNearest_CGS6, // 最近点を描画_曲線
    //DrawSurfaceNearest_CGS7, // 最近点を描画_曲面
    //DrawSplitCurve_CGS8,   // 分割曲線を描画
    //DrawSplitSurface_CGS8, // 分割曲面を描画
	//DrawIntersectCurveSurface_CGS8_one, // 曲線と曲面の1交点取得
    //DrawIntersectCurveSurface_CGS8, // 曲線と曲面の交点取得
    DrawUV_CGS9,
    DrawXYZ_CGS9,
    CalcArea_CGS9,
    //DrawAllKind,
};

// ループを構成するエッジ群の端を補正
void AdjustLoopEdges(std::vector<std::vector<Point<double>>>& edges)
{
    int edge_cnt = edges.size();

    // エッジ群の始端と終端が一致するように補正
    for (int i = 0; i < edge_cnt; ++i)
    {
        int edge_pnt_cnt = edges[i].size();
        int next_edge_idx = (i + 1 >= edges.size()) ? i + 1 : 0;

        for (int j = 0; j < edge_pnt_cnt - 1; ++j)
        {
            if (j == edges[i].size() - 1)
            {
                // エッジの終端と次のエッジの始端の中点を補正点とする
                Point<double> middle;
                middle.x = edges[i][j].x + edges[next_edge_idx][0].x;
                middle.y = edges[i][j].y + edges[next_edge_idx][0].y;

                edges[i][j] = middle;
                edges[next_edge_idx][0] = middle;
            }
        }
    }
}
// ループを構成するエッジ群(向き付け済み)からループ面積を取得
double GetLoopArea(int base, const std::vector<std::vector<Point<double>>>& edges)
{
    double area = 0.0; // 面積(符号付)
    int edge_cnt = edges.size();

    for (int i = 0; i < edge_cnt; ++i)
    {
        int edge_pnt_cnt = edges[i].size();

        for (int j = 0; j < edge_pnt_cnt - 1; ++j)
        {
            double height = edges[i][j].x - edges[i][j + 1].x;
            double upper = edges[i][j].y - base;
            double lower = edges[i][j + 1].y - base;

            // 台形面積
            area += (upper + lower) * height * 0.5;
        }
    }
}

void DrawUV_CGS9()
{
    auto reader = std::make_unique<KjsReader>();

    // 参照曲線
    std::shared_ptr<BsplineCurve> curve1((BsplineCurve *)reader->GetObjectFromFile(curve9_1_name));
    std::shared_ptr<BsplineCurve> curve2((BsplineCurve *)reader->GetObjectFromFile(curve9_2_name));
    std::shared_ptr<BsplineCurve> curve3((BsplineCurve *)reader->GetObjectFromFile(curve9_3_name));
    std::shared_ptr<BsplineCurve> curve4((BsplineCurve *)reader->GetObjectFromFile(curve9_4_name));
    // 参照曲面
    std::shared_ptr<BsplineSurface> surf((BsplineSurface *)reader->GetObjectFromFile(surf1_name));

    std::vector<Point<double>> uv_params_list[4];
    auto uv_curve1 = GetOnSurfaceUVParams(curve1, surf, uv_params_list[0]);
   // auto uv_curve1_copy = GetOnSurfaceUVParams(curve1, surf, uv_params_list[0]);
    auto uv_curve2 = GetOnSurfaceUVParams(curve2, surf, uv_params_list[1]);
    auto uv_curve3 = GetOnSurfaceUVParams(curve3, surf, uv_params_list[2]);
    auto uv_curve4 = GetOnSurfaceUVParams(curve4, surf, uv_params_list[3]);
    {
        // UVパラメータ描画
        glColor3dv(Color::red);
        glPointSize(3.0);
        glBegin(GL_POINTS);
        {
            for (const auto& uv_params : uv_params_list)
            {
                //for (const auto& uv_param : uv_params)
                //    glVertex3d(Vector3d(uv_param.x, uv_param.y, 0));
            }
        }
        glEnd();
    }

    if (isFirst)
    {
        //test_scene->AddObject(curve1->GetName(), curve1);
        //test_scene->AddObject(curve2->GetName(), curve2);
        //test_scene->AddObject(curve3->GetName(), curve3);
        //test_scene->AddObject(curve4->GetName(), curve4);
        test_scene->AddObject(surf->GetName(), surf);

        uv_curve1->SetColor(Color::light_blue);
        uv_curve2->SetColor(Color::orange);
        uv_curve3->SetColor(Color::pink);
        uv_curve4->SetColor(Color::red);
        ////uv_curve1_copy->SetColor(Color::blue);
        ////uv_curve1_copy->ReverseFault();

        ////uv_curve1->Reverse();
        ////uv_curve4->Reverse();

        ////test_scene->AddObject(uv_curve1->GetName(), uv_curve1_copy);
        test_scene->AddObject(uv_curve1->GetName(), uv_curve1);
        test_scene->AddObject(uv_curve2->GetName(), uv_curve2);
        test_scene->AddObject(uv_curve3->GetName(), uv_curve3);
        test_scene->AddObject(uv_curve4->GetName(), uv_curve4);
    }
}
void DrawXYZ_CGS9()
{

}
void CalcArea_CGS9()
{


}

// すべてのタイプの曲線/曲面を描画
void DrawAllKind()
{
    auto reader = std::make_unique<KjsReader>();

    // 参照曲線
    std::shared_ptr<BsplineCurve> curve1((BsplineCurve *)reader->GetObjectFromFile(curve1_name));
    // 参照曲面
    std::shared_ptr<BsplineSurface> surf1((BsplineSurface *)reader->GetObjectFromFile(surf1_name));
    // 参照曲線
    std::shared_ptr<BezierCurve> curve2((BezierCurve *)reader->GetObjectFromFile(bezierCurveB_name));
    // 参照曲面
    std::shared_ptr<BezierSurface> surf2((BezierSurface *)reader->GetObjectFromFile(bezierSurfA_name));

    if (isFirst)
    {
        test_scene->AddObject(curve1->GetName(), curve1);
        test_scene->AddObject(curve2->GetName(), curve2);

        test_scene->AddObject(surf1->GetName(), surf1);
        test_scene->AddObject(surf2->GetName(), surf2);
    }
}

// 曲線と曲面の交点群取得
void DrawIntersectCurveSurface_CGS8()
{
    auto reader = std::make_unique<KjsReader>();

    // 参照曲線
    std::shared_ptr<BsplineCurve> curve((BsplineCurve *)reader->GetObjectFromFile(curveG_name));
    // 参照曲面
    std::shared_ptr<BsplineSurface> surf((BsplineSurface *)reader->GetObjectFromFile(surf1_name));

    // 曲線分割
    std::vector<std::shared_ptr<Curve>> split_curves;
    curve->GetDevidedCurves(10, split_curves);

    // 曲面分割
    std::vector<std::vector<std::shared_ptr<Surface>>> split_surfaces;
    surf->GetDevidedSurfaces(10, 10, split_surfaces, surf->_color);

    // 交点取得
    {
        auto solver = std::make_unique<IntersectSolver>(1.0e-6, IntersectSolver::Algo::BoxInterfere);
        solver->SetPair(curve, surf);

        // 交点描画
        {
            std::vector<Vector3d> intersects = solver->GetIntersects();

            glColor3dv(Color::red);
            glPointSize(12.0);
            for (const auto &intersect : intersects)
            {
                glBegin(GL_POINTS);
                glVertex3d(intersect);
                glEnd();
            }
        }
    }

    if (isFirst)
    {
        for (const auto &c : split_curves)
            test_scene->AddObject(c->GetName(), c);

        for (const auto &us : split_surfaces)
        {
            for (const auto &vs : us)
            {
                test_scene->AddObject(vs->GetName(), vs);
            }
        }

        //test_scene->AddObject(curve->GetName(), curve);
        //test_scene->AddObject(surf->GetName(), surf);
    }
}
// 曲線と曲面の1交点取得
void DrawIntersectCurveSurface_CGS8_one()
{
	auto reader = std::make_unique<KjsReader>();

	// 参照曲線
	std::shared_ptr<BsplineCurve> curve((BsplineCurve *)reader->GetObjectFromFile(curveS_name));
	// 参照曲面
	std::shared_ptr<BsplineSurface> surf((BsplineSurface *)reader->GetObjectFromFile(surf1_name));

	// 曲線分割
	std::vector<std::shared_ptr<Curve>> split_curves;
	curve->GetDevidedCurves(10, split_curves);

	// 曲面分割
	std::vector<std::vector<std::shared_ptr<Surface>>> split_surfaces;
	surf->GetDevidedSurfaces(10, 10, split_surfaces, surf->_color);

	// 交点取得
	{
		auto solver = std::make_unique<IntersectSolver>(1.0e-6, IntersectSolver::Algo::BoxInterfere);
		solver->SetPair(curve, surf);

		// 交点描画
		{
			Vector3d intersect = solver->GetIntersect();

			glColor3dv(Color::red);
			glPointSize(12.0);
			{
				glBegin(GL_POINTS);
				glVertex3d(intersect);
				glEnd();
			}
		}
	}

	if (isFirst)
	{
		for (const auto &c : split_curves)
			test_scene->AddObject(c->GetName(), c);

		for (const auto &us : split_surfaces)
		{
			for (const auto &vs : us)
			{
				test_scene->AddObject(vs->GetName(), vs);
			}
		}
	}
}

// 分割曲面を描画
void DrawSplitSurface_CGS8()
{
    auto reader = std::make_unique<KjsReader>();

    // 分割対象曲線
    std::shared_ptr<BsplineSurface> surf1((BsplineSurface *)reader->GetObjectFromFile(surf1_name));

    //surf1->AddKnot(ParamUV::V, 1);
    //surf1->AddKnot(ParamUV::V, 1);
    //surf1->AddKnot(ParamUV::U, 1);

    std::vector<vector<std::shared_ptr<Surface>>> devided;
    vector<double> u_test_params = {0.4, 1.5, 2.3, 2.8};
    vector<double> v_test_params = {0.4, 1.5, 1.7};
    surf1->GetDevidedSurfaces(u_test_params, v_test_params, devided, surf1->_color);

    if (isFirst)
    {
        //test_scene->AddObject(surf1->GetName(), surf1);

        for (const auto &us : devided)
        {
            for (const auto &vs : us)
            {
                test_scene->AddObject(vs->GetName(), vs);
            }
        }
    }
}

// 分割曲線描画
void DrawSplitCurve_CGS8()
{
    auto reader = std::make_unique<KjsReader>();

    // 分割対象曲線
    std::shared_ptr<BsplineCurve> curve1((BsplineCurve *)reader->GetObjectFromFile(curve1_name));

    // ノット追加曲線
    std::shared_ptr<BsplineCurve> curve1_clone((BsplineCurve *)reader->GetObjectFromFile(curve1_name));
    curve1_clone->SetColor(Color::red);
    // curve1_clone->AddKnot(3);
    // curve1_clone->AddKnot(3);
    // curve1_clone->AddKnot(3);
    // curve1_clone->AddKnot(3);
    // curve1_clone->AddKnot(1.5);
    // curve1_clone->AddKnot(1.5);
    // curve1_clone->AddKnot(1.5);

    // // 基底関数(無理やり)
    // double knot[20] = {0, 0, 0, 0, 1,1.5, 1.5, 1.5, 2, 3 ,4, 5, 5, 5, 5};
    // DrawBsplineFunc(4, 11, 15, knot, 0, 5);

    auto test_params = vector<double>{1.5, 3, 3.2, 3.4, 4.3, 4.7};

    // 分割曲線
    std::vector<std::shared_ptr<Curve>> split_curves;
    curve1_clone->GetDevidedCurves(test_params, split_curves);
    //auto s = curve1_clone->Get2DevidedCurves(3);
    //auto c = (s.first)->Get2DevidedCurves(1.5);

    //(c.first)->SetName("left");
    //(c.second)->SetName("right");

    if (isFirst)
    {
        //test_scene->AddObject(curve1_name, curve1);
        //test_scene->AddObject(curve1_name+"add_1knot", curve1_clone);
        //test_scene->AddObject((c.first)->GetName(), c.first);
        //test_scene->AddObject((c.second)->GetName(), c.second);
        for (const auto &c : split_curves)
            test_scene->AddObject(c->GetName(), c);
    }
}

// 参照曲線から最近点を求めて描画
void DrawCurveNearest_CGS6()
{
    auto reader = std::make_unique<KjsReader>();

    // 対象曲線/曲面
    std::shared_ptr<BsplineCurve> curve1((BsplineCurve *)reader->GetObjectFromFile(curve1_name));
    // 参照曲線
    std::shared_ptr<BsplineCurve> curveC((BsplineCurve *)reader->GetObjectFromFile(curveC_name));

    //// 対象曲線をセグメント8分割した点群
    //{
    //    vector<Vector3d> passPnts, segPassPnts;
    //    passPnts = curve1->GetPositionVectorsByKnots(8);
    //    segPassPnts = curve1->GetPositionVectorsByKnots();
    //    DrawPoints(segPassPnts, Color::pink, 10);
    //    DrawPoints(passPnts, Color::green, 10);
    //}

    // 参照点群を取得
    vector<Vector3d> ref_pnts;
    curveC->GetPositionVectors(ref_pnts, 20); // 20分割して21点を取る

    // 最近点取得
    vector<NearestPointInfoC> nearest_pnts;
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(curve1->GetNearestPointInfoFromRef(ref_pnts[i]));

    // 描画
    {
        glLineWidth(1.0);
        glBegin(GL_LINES);
        {
            // 参照点と最近点を結ぶ線
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
            // 参照点
            glColor4dv(Color::red);
            for (int i = 0; i < (int)ref_pnts.size(); i++)
                glVertex3d(ref_pnts[i]);

            // 最近点
            glColor4dv(Color::light_green);
            for (int i = 0; i < (int)nearest_pnts.size(); i++)
                glVertex3d(nearest_pnts[i].nearestPnt);
        }
        glEnd();
    }

    // 詳細をcsv吐き出し
    ExportCsv_NearestInfoCurve("nearest_pnt_CGS6.csv", nearest_pnts, true);

    if (isFirst)
    {
        test_scene->AddObject(curve1_name, curve1);
        test_scene->AddObject(curveC_name, curveC);
    }
}

// 参照曲面から最近点を求めて描画
void DrawSurfaceNearest_CGS7()
{
    auto reader = std::make_unique<KjsReader>();

    // 対象曲線/曲面
    std::shared_ptr<BsplineSurface> surf1((BsplineSurface *)reader->GetObjectFromFile(surf1_name));
    // 参照曲線
    std::shared_ptr<BsplineCurve> curveS((BsplineCurve *)reader->GetObjectFromFile(curveS_name));

    // 参照点群(21点)を取得
    vector<Vector3d> ref_pnts;
    curveS->GetPositionVectors(ref_pnts, 20);
    vector<NearestPointInfoS> nearest_pnts; // 最近点群

    clock_t start = clock();

    // 最近点取得
    for (const auto &ref : ref_pnts)
        //nearest_pnts.push_back(surf1->GetNearestPointInfoFromRef(ref, Surface::Project));
        nearest_pnts.push_back(surf1->GetNearestPointInfoFromRef(ref, Surface::Isoline));

    clock_t end = clock();
    cout << "トレランス : " << EPS::NEAREST << "  最近点取得(21点)時間 = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

    vector<vector<Vector3d>> startPnts; // 開始点群
    int seg_split_u = 1;
    int seg_split_v = 1;
    surf1->GetPointsByKnots(startPnts, seg_split_u, seg_split_v);

    // 描画
    {
        glLineWidth(1.0);
        glBegin(GL_LINES);
        {
            // 参照点と最近点を結ぶ線分
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
            // 参照点
            glColor4dv(Color::red);
            for (int i = 0; i < (int)ref_pnts.size(); i++)
                glVertex3d(ref_pnts[i]);

            // 最近点
            glColor4dv(Color::light_green);
            for (int i = 0; i < (int)nearest_pnts.size(); i++)
                glVertex3d(nearest_pnts[i].nearestPnt);
        }
        glEnd();

        glPointSize(10.0);
        glBegin(GL_POINTS);
        {
            // 開始点
            glColor4dv(Color::pink);
            for (const auto &startRow : startPnts)
                for (const auto &start : startRow)
                    glVertex3d(start);
        }
        glEnd();
    }

    // 詳細をcsv吐き出し
    ExportCsv_NearestInfoSurface("nearest_pnt_CGS7.csv", nearest_pnts, true);

    if (isFirst)
    {
        test_scene->AddObject(surf1_name, surf1);
        test_scene->AddObject(curveS_name, curveS);
    }
}

// 近似して曲面を描画
void DrawApproxSurface_CGS5()
{
    auto reader = std::make_unique<KjsReader>();

    std::shared_ptr<BsplineSurface> surf((BsplineSurface *)reader->GetObjectFromFile(surf1_name));

    // 近似曲面
    std::unique_ptr<Surface> surf_knot_remake, surf_knot_split_remake;

    // 曲面の近似(ノット位置のみ)
    {
        vector<vector<Vector3d>> passPnts;
        surf->GetPointsByKnots(passPnts, 1, 1);
        for (const auto &pnts : passPnts)
            DrawPoints(pnts, Color::green, 10);

        surf_knot_remake = surf->GetSurfaceFromPoints(passPnts, Color::blue_alpha, 20);
    }
    // 曲面の近似(ノット位置のみ + セグメント位置3分割)
    {
        vector<vector<Vector3d>> passPnts;
        surf->GetPointsByKnots(passPnts, 3, 3);
        for (const auto &pnts : passPnts)
            DrawPoints(pnts, Color::pink, 10);

        surf_knot_split_remake = surf->GetSurfaceFromPoints(passPnts, Color::orange_alpha, 20);
    }

    if (isFirst)
    {
        test_scene->AddObject(surf1_name, surf);
        //test_scene->AddObject(surf_knot_remake);
        test_scene->AddObject("surf1_knot_split_remake", std::move(surf_knot_split_remake));
    }
}

// 近似して曲線を描画
void DrawApproxCurve_CGS4()
{
    auto reader = std::make_unique<KjsReader>();

    shared_ptr<BsplineCurve> curve1((BsplineCurve *)reader->GetObjectFromFile("CGS_bspline_curve_1.kjs"));
    shared_ptr<BsplineCurve> curve2((BsplineCurve *)reader->GetObjectFromFile("CGS_bspline_curve_2.kjs"));

    // 近似曲線
    std::shared_ptr<Curve> curve1_remake, curve2_remake;
    std::shared_ptr<Curve> curve1_remake_split, curve2_remake_split;

    // 曲線1の近似(ノット位置のみ)
    {
        vector<Vector3d> passPnts;
        passPnts = curve1->GetPositionVectorsByKnots();
        DrawPoints(passPnts, Color::green, 10);

        curve1_remake = curve1->GetCurveFromPoints(passPnts, Color::red, 3);
    }
    // 曲線1の近似(ノット位置のみ + セグメント位置3分割)
    {
        vector<Vector3d> passPnts;
        passPnts = curve1->GetPositionVectorsByKnots(3);
        vector<Vector3d> passPntsOnlyKnot;
        passPntsOnlyKnot = curve1->GetPositionVectorsByKnots();
        DrawPoints(passPntsOnlyKnot, Color::green, 10);
        DrawPoints(passPnts, Color::pink, 10);

        curve1_remake_split = curve1->GetCurveFromPoints(passPnts, Color::orange, 3);
    }

    printf("曲線1と近似曲線の相違距離平均\n");
    printf("ノット位置のみ:    %f\n", curve1->CalcDifferency(curve1_remake.get()));
    printf("セグメント位置も:  %f\n", curve1->CalcDifferency(curve1_remake_split.get()));
    printf("\n");

    printf("曲線1と近似曲線の相違距離平均 Ver.2 \n");
    printf("ノット位置のみ:    %f\n", curve1->CalcDifferency2(curve1_remake.get()));
    printf("セグメント位置も:  %f\n", curve1->CalcDifferency2(curve1_remake_split.get()));
    printf("\n");

    printf("曲線1と近似曲線の一番遠ざかる距離 \n");
    printf("ノット位置のみ:    %f\n", curve1->CalcFarthestDistant(curve1_remake.get()));
    printf("セグメント位置も:  %f\n", curve1->CalcFarthestDistant(curve1_remake_split.get()));
    printf("\n");

    // 曲線2の近似(ノット位置のみ)
    {
        vector<Vector3d> passPnts;
        passPnts = curve2->GetPositionVectorsByKnots();
        DrawPoints(passPnts, Color::green, 10);

        curve2_remake = curve2->GetCurveFromPoints(passPnts, Color::red, 3);
    }
    // 曲線2の近似(ノット位置のみ + セグメント位置3分割)
    {
        vector<Vector3d> passPnts;
        passPnts = curve2->GetPositionVectorsByKnots(3);
        vector<Vector3d> passPntsOnlyKnot;
        passPntsOnlyKnot = curve2->GetPositionVectorsByKnots();
        DrawPoints(passPntsOnlyKnot, Color::green, 10);
        DrawPoints(passPnts, Color::pink, 10);

        curve2_remake_split = curve2->GetCurveFromPoints(passPnts, Color::orange, 3);
    }

    printf("曲線2と近似曲線の相違距離平均\n");
    printf("ノット位置のみ:    %f\n", curve2->CalcDifferency(curve2_remake.get()));
    printf("セグメント位置も:  %f\n", curve2->CalcDifferency(curve2_remake_split.get()));

    printf("曲線2と近似曲線の相違距離平均 Ver.2 \n");
    printf("ノット位置のみ:    %f\n", curve2->CalcDifferency2(curve2_remake.get()));
    printf("セグメント位置も:  %f\n", curve2->CalcDifferency2(curve2_remake_split.get()));

    printf("曲線2と近似曲線の一番遠ざかる距離 \n");
    printf("ノット位置のみ:    %f\n", curve2->CalcFarthestDistant(curve2_remake.get()));
    printf("セグメント位置も:  %f\n", curve2->CalcFarthestDistant(curve2_remake_split.get()));

    if (isFirst)
    {
        test_scene->AddObject(curve1_name, curve1);
        test_scene->AddObject("curve1_remake", std::move(curve1_remake));
        test_scene->AddObject("curve1_remake_split", std::move(curve1_remake_split));

        test_scene->AddObject(curve2_name, curve2);
        test_scene->AddObject("curve2_remake", std::move(curve2_remake));
        test_scene->AddObject("curve2_remake_split", std::move(curve2_remake_split));
    }
}

// Nurbs曲面で円柱を描く
void DrawCylinder_CGS3()
{
    // よくないけど無理やり求めた
    double cp1_p[2] = {5, 5 * sqrt(3)};
    RotateCoord2DAroundOrigin(cp1_p, (double)2 / 3 * M_PI);

    ControlPoint cp0[9]{
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 5 * sqrt(3), 0, (double)1 / 2),
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),

        ControlPoint(5, 0, 10, 1.0),
        ControlPoint(5, 5 * sqrt(3), 10, (double)1 / 2),
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
    };
    ControlPoint cp1[9]{
        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 0, 1.0),
        ControlPoint(cp1_p[0], cp1_p[1], 0, (double)1 / 2),
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),

        ControlPoint(-(double)5 / 2, 5 * (sqrt(3) / 2), 10, 1.0),
        ControlPoint(cp1_p[0], cp1_p[1], 10, (double)1 / 2),
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
    };
    ControlPoint cp2[9]{
        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 0, 1.0),
        ControlPoint(5, -5 * sqrt(3), 0, (double)1 / 2),
        ControlPoint(5, 0, 0, 1.0),

        ControlPoint(-(double)5 / 2, -5 * (sqrt(3) / 2), 10, 1.0),
        ControlPoint(5, -5 * sqrt(3), 10, (double)1 / 2),
        ControlPoint(5, 0, 10, 1.0),
    };

    double knotU[6] = {0, 0, 0, 1, 1, 1};
    double knotV[4] = {0, 0, 1, 1};

    shared_ptr<NurbsSurface> surf0(new NurbsSurface(3, 2, cp0, 3, 2, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf1(new NurbsSurface(3, 2, cp1, 3, 2, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf2(new NurbsSurface(3, 2, cp2, 3, 2, knotU, knotV, Color::green_alpha));

    if (isFirst)
    {
        test_scene->AddObject("cylinder_part0", surf0);
        test_scene->AddObject("cylinder_part1", surf1);
        test_scene->AddObject("cylinder_part2", surf2);
    }
}

// Nurbs曲面で球を描く
void DrawSphere_CGS3()
{
    ControlPoint cp0[9]{
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
    ControlPoint cp1[9]{
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
    ControlPoint cp2[9]{
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
    ControlPoint cp3[9]{
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
    ControlPoint cp4[9]{
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
    ControlPoint cp5[9]{
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
    ControlPoint cp6[9]{
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
    ControlPoint cp7[9]{
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

    double knotU[6] = {0, 0, 0, 1, 1, 1};
    double knotV[6] = {0, 0, 0, 1, 1, 1};

    shared_ptr<NurbsSurface> surf0(new NurbsSurface(3, 3, cp0, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf1(new NurbsSurface(3, 3, cp1, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf2(new NurbsSurface(3, 3, cp2, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf3(new NurbsSurface(3, 3, cp3, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf4(new NurbsSurface(3, 3, cp4, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf5(new NurbsSurface(3, 3, cp5, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf6(new NurbsSurface(3, 3, cp6, 3, 3, knotU, knotV, Color::green_alpha));
    shared_ptr<NurbsSurface> surf7(new NurbsSurface(3, 3, cp7, 3, 3, knotU, knotV, Color::green_alpha));

    if (isFirst)
    {
        test_scene->AddObject("sphere_part0", surf0);
        test_scene->AddObject("sphere_part1", surf1);
        test_scene->AddObject("sphere_part2", surf2);
        test_scene->AddObject("sphere_part3", surf3);
        test_scene->AddObject("sphere_part4", surf4);
        test_scene->AddObject("sphere_part5", surf5);
        test_scene->AddObject("sphere_part6", surf6);
        test_scene->AddObject("sphere_part7", surf7);
    }
}

// Nurbs曲線で円を描く
void DrawCircle_CGS3()
{
    ControlPoint cp0[3]{
        ControlPoint(5, 0, 0, 1),
        ControlPoint(5, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1),
    };
    ControlPoint cp1[3]{
        ControlPoint(0, 5, 0, sqrt(2)),
        ControlPoint(-5, 5, 0, 1),
        ControlPoint(-5, 0, 0, sqrt(2)),
    };
    ControlPoint cp2[3]{
        ControlPoint(-5, 0, 0, sqrt(2)),
        ControlPoint(-5, -5, 0, 1),
        ControlPoint(0, -5, 0, sqrt(2)),
    };
    ControlPoint cp3[3]{
        ControlPoint(0, -5, 0, sqrt(2)),
        ControlPoint(5, -5, 0, 1),
        ControlPoint(5, 0, 0, sqrt(2)),
    };

    double knot[6] = {0, 0, 0, 1, 1, 1};

    shared_ptr<NurbsCurve> curve0(new NurbsCurve(3, cp0, 3, knot, Color::blue, 2.0));
    shared_ptr<NurbsCurve> curve1(new NurbsCurve(3, cp1, 3, knot, Color::blue, 2.0));
    shared_ptr<NurbsCurve> curve2(new NurbsCurve(3, cp2, 3, knot, Color::blue, 2.0));
    shared_ptr<NurbsCurve> curve3(new NurbsCurve(3, cp3, 3, knot, Color::blue, 2.0));

    if (isFirst)
    {
        test_scene->AddObject("circle_part0", curve0);
        test_scene->AddObject("circle_part1", curve1);
        test_scene->AddObject("circle_part2", curve2);
        test_scene->AddObject("circle_part3", curve3);
    }
}

// 基底関数およびその関数を使ってBスプライン曲線を描画する
// 第1回課題
static double knot_a[10] = {-3, -2, -1, 0, 1, 2, 3, 4, 5, 6};
static double knot_b[10] = {0, 0, 0, 0, 1, 2, 3, 3, 3, 3};
static double knot_c[10] = {0, 0, 0, 0, 1, 1, 3, 3, 3, 3};
void DrawBsplineFunctions()
{
    // 別のとこでディスプレイリストをstaticで使ってるので同時には書けない
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

    // 制御点込みで描画
    A->DrawAsItIsWithCPs();
    B->DrawAsItIsWithCPs();
    C->DrawAsItIsWithCPs();
}

// テスト描画
void TestDraw()
{
    if (isRegistered)
    {
        // 全ディスプレイリストコール
        for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
        {
            if (displayLists[i])
                glCallList(displayLists[i]);
        }

        // テストシーン描画
        test_scene->Draw();
    }
    else
    {
        Error::ShowMessage(
            "テスト用ディスプレイリスト未作成",
            "自動で作成しますが, 適切な場所に登録関数を置くことを推奨します.");

        TestRegister();
        glutPostRedisplay();
    }
}

// ディスプレイリストに描画を登録する(描画前に手動で呼び出すことを推奨)
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
