#include "MakeUV.h"

bool CheckDistant(const std::shared_ptr<BsplineCurve>& curve, const std::shared_ptr<BsplineCurve>& curve_on_surf, const double eps)
{
    return curve->IsDifferentAtLeast(curve_on_surf.get(), eps);
}

// 曲面上に近似曲線が乗るようなUVパラメータ群を取得
std::shared_ptr<BsplineCurve> GetOnSurfaceUVParams(const std::shared_ptr<BsplineCurve>& curve, const std::shared_ptr<BsplineSurface>& surf,
    std::vector<Point<double>>& uv_params)
{
    int seg_split = 2;

    while (true)
    {
        // 既に設定されていれば削除
        if (uv_params.size() > 0)
        {
            uv_params.clear();
            uv_params.shrink_to_fit();
        }

        double tole = EPS::NEAREST; // 近似トレランス

        // XYZの離散点 (セグメント分割)
        std::vector<Point3dC> c_pts = curve->GetPointsByKnots(seg_split);
        /*std::vector<Vector3d> c_pts;
        curve->GetPositionVectors(c_pts, seg_split);
*/
        // 離散点から曲面への最近点取得
        std::vector<NearestPointInfoS> nearest_pts;
        {
            for (auto& pts : c_pts)
            {
                nearest_pts.push_back(surf->GetNearestPointInfoFromRef((Vector3d)pts));
            }
        }
        // 近似トレランスを計算
        {
            double max_dist = -DBL_MAX;
            for (const auto& pts : nearest_pts)
            {
                if (pts.dist > max_dist)
                    max_dist = pts.dist;
            }
            tole = std::fmax(tole, max_dist);
        }


        // 最近点からUVパラメータ取得
        for (const auto& pnt : nearest_pts)
        {
            Point<double> uv_param;
            uv_param.x = pnt.paramU;
            uv_param.y = pnt.paramV;
            uv_params.push_back(uv_param);
        }

        {
            // UVパラメータ群を3次元点に変換
            std::vector<Vector3d> uv_pts(uv_params.size());
            {
                for (int i = 0; i < uv_pts.size(); ++i)
                    uv_pts[i] = Vector3d(uv_params[i].x, uv_params[i].y, 0);
            }
            // UV曲線を取得
            auto uv_curve = static_pointer_cast<BsplineCurve>(GetBsplineCurveFromPoints(uv_pts, 4, Color::green, 2));

            // UV曲線から面上線を取得
            //std::vector<Point3dC> pts_on_uv_curve = uv_curve->GetPointsByKnots(seg_split);
   /*         std::vector<Vector3d> pts_on_uv_curve;
            uv_curve->GetPositionVectors(pts_on_uv_curve, seg_split);*/

            std::vector<Vector3d> pts_on_surf;
            for (auto& pnt : uv_pts)
                pts_on_surf.push_back(surf->GetPositionVector(((Vector3d)pnt).X, ((Vector3d)pnt).Y));
            auto curve_on_surf = static_pointer_cast<BsplineCurve>(GetBsplineCurveFromPoints(pts_on_surf, 4, Color::blue, 2));

            if (CheckDistant(curve, curve_on_surf, tole))
            {
                cout << seg_split << endl;
                return curve_on_surf;
            }
        }

        seg_split++;
    }
}
