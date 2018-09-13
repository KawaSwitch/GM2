#pragma once

#include <vector>
#include <memory>
#include "BsplineCurve.h"

// 位相要素 エッジ
class Edge
{
private:
    // 幾何要素
    std::shared_ptr<BsplineCurve> _xyz_curve;
    std::shared_ptr<BsplineCurve> _uv_curve;

public:
    // getter
    std::shared_ptr<BsplineCurve> GetXYZCurve() const { return _xyz_curve; }
    std::shared_ptr<BsplineCurve> GetUVCurve() const { return _uv_curve; }

    // XYZ曲線上の点群を取得
    // split: 曲線分割数 (描画範囲)
    void GetOnXYZCurvePoints(const int split, std::vector<Vector3d>& pnts) const
    {
        pnts.clear(); pnts.shrink_to_fit();
        _xyz_curve->GetPositionVectors(pnts, split);
    }

    // UV曲線上の点群を取得
    // split: 曲線分割数 (描画範囲)
    void GetOnUVCurvePoints(const int split, std::vector<Point<double>>& pnts) const
    {
        pnts.clear(); pnts.shrink_to_fit();
        
        std::vector<Vector3d> temp;
        _uv_curve->GetPositionVectors(temp, split);

        // 2次元点に変換 (GetPositionVectorsメソッドは3次元(z = 0)を返却)
        pnts.reserve(temp.size());
        for (auto& pnt : temp)
            pnts.emplace_back(Point<double>(pnt.X, pnt.Y));
    }

    // エッジを反転する
    void Reverse()
    {
        _xyz_curve->Reverse();
        _uv_curve->Reverse();
    }

    Edge(const std::shared_ptr<BsplineCurve> xyz_curve, const std::shared_ptr<BsplineCurve> uv_curve)
    {
        _xyz_curve = xyz_curve;
        _uv_curve = uv_curve;
    }
};

// 位相要素 ループ
class Loop
{
private:
    std::vector<Edge> _edges;
    int _edge_cnt;

    enum LoopDirection
    {
        CW, // 右回り
        CCW, // 左回り
    };
    LoopDirection _loopDirection;

    // エッジからループを構成
    void AdjustForLoop()
    {
        this->MakeLoop();
        this->FixLoop(); // 補正
    }
    // エッジの向きを確かめループにする
    // 予め隣のエッジが順番に並んでいることが必要
    void MakeLoop()
    {
        const double r = 0.1; // 同一点と判定する判定円の半径
        const int split = 1; // 1セグメントあれば十分

        for (int i = 0; i < _edge_cnt; ++i)
        {
            std::vector<Point<double>> edge_pnts; // エッジを構成する点群
            _edges[i].GetOnUVCurvePoints(split, edge_pnts);
            int edge_pnt_cnt = (int)edge_pnts.size();

            int pre_edge_idx = (i - 1 >= 0) ? i - 1 : _edge_cnt - 1;
            std::vector<Point<double>> pre_edge_pnts; // 前のエッジを構成する点群
            _edges[pre_edge_idx].GetOnUVCurvePoints(split, pre_edge_pnts);

            {
                if (_loopDirection == LoopDirection::CCW)
                {
                    // 今の始点と前のエッジの終点が同じであればループOK
                    // そうでなければエッジ反転
                    if (!IsNear(edge_pnts[0], pre_edge_pnts[1], r))
                        _edges[i].Reverse();
                }
                else
                {
                    throw; // TODO: 実装
                }
            }
        }
    }
    // エッジの端が繋がるように補正
    void FixLoop()
    {
        int split = 10; // 1でいいかも

        for (int i = 0; i < _edge_cnt; ++i)
        {
            std::vector<Point<double>> edge_pnts; // エッジを構成する点群
            _edges[i].GetOnUVCurvePoints(split, edge_pnts);
            int edge_pnt_cnt = (int)edge_pnts.size();

            int next_edge_idx = (i + 1 < _edge_cnt) ? i + 1 : 0;
            std::vector<Point<double>> next_edge_pnts; // 次のエッジを構成する点群
            _edges[next_edge_idx].GetOnUVCurvePoints(split, next_edge_pnts);

            {
                // エッジの終端と次のエッジの始端の中点を補正点とする
                double middleX = (edge_pnts[edge_pnt_cnt - 1].x + next_edge_pnts[0].x) / 2;
                double middleY = (edge_pnts[edge_pnt_cnt - 1].y + next_edge_pnts[0].y) / 2;
                Point<double> middle(middleX, middleY);

                edge_pnts[edge_pnt_cnt - 1] = middle; // 終端
                next_edge_pnts[0] = middle; // 始端
            }
        }
    }

    // エッジごとの折れ点の端が繋がるように補正
    void FixPolyline(std::vector<std::vector<Point<double>>>& polylines) const
    {
        for (int i = 0; i < _edge_cnt; ++i)
        {
            int edge_pnt_cnt = (int)polylines[i].size();
            int next_edge_idx = (i + 1 < _edge_cnt) ? i + 1 : 0;

            // エッジの終端と次のエッジの始端の中点を補正点とする
            {
                double middleX = (polylines[i][edge_pnt_cnt - 1].x + polylines[next_edge_idx][0].x) / 2;
                double middleY = (polylines[i][edge_pnt_cnt - 1].y + polylines[next_edge_idx][0].y) / 2;
                Point<double> middle(middleX, middleY);

                polylines[i][edge_pnt_cnt - 1] = middle; // 終端
                polylines[next_edge_idx][0] = middle; // 始端
            }
        }
    }

public:

    // ループUV多角形の面積を求める
    double GetArea(const int baseline = 0) const
    {
        double area = 0.0; // 面積(符号付)
        const int split = 20; // エッジ分割数

        // エッジからUV折れ線のループ多角形を作成する
        std::vector<std::vector<Point<double>>> polylines(_edge_cnt); // 折れ線群
        {
            for (int i = 0; i < _edge_cnt; ++i)
                _edges[i].GetOnUVCurvePoints(split, polylines[i]);

            // 端がつながるように補正
            this->FixPolyline(polylines);
        }

        // ループ多角形の面積を求める
        for (int i = 0; i < _edge_cnt; ++i)
        {
            int edge_pnt_cnt = (int)polylines[i].size();

            for (int j = 0; j < edge_pnt_cnt - 1; ++j)
            {
                double height = polylines[i][j].x - polylines[i][j + 1].x;
                double upper = polylines[i][j].y - baseline;
                double lower = polylines[i][j + 1].y - baseline;

                // 台形面積
                area += (upper + lower) * height * 0.5;
            }

            cout << i + 1 << "番ループまでの面積: " << area << endl;
        }

        return area;
    }

    Loop(const std::vector<Edge>& edges)
    {
        _edges = edges;
        _edge_cnt = (int)_edges.size();
        _loopDirection = LoopDirection::CCW; // 左回り固定

        this->AdjustForLoop(); // 向き付けを行い補正
    }
};
