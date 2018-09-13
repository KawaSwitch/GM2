#pragma once

#include <vector>
#include <memory>
#include "BsplineCurve.h"

// �ʑ��v�f �G�b�W
class Edge
{
private:
    // �􉽗v�f
    std::shared_ptr<BsplineCurve> _xyz_curve;
    std::shared_ptr<BsplineCurve> _uv_curve;

public:
    // getter
    std::shared_ptr<BsplineCurve> GetXYZCurve() const { return _xyz_curve; }
    std::shared_ptr<BsplineCurve> GetUVCurve() const { return _uv_curve; }

    // XYZ�Ȑ���̓_�Q���擾
    // split: �Ȑ������� (�`��͈�)
    void GetOnXYZCurvePoints(const int split, std::vector<Vector3d>& pnts) const
    {
        pnts.clear(); pnts.shrink_to_fit();
        _xyz_curve->GetPositionVectors(pnts, split);
    }

    // UV�Ȑ���̓_�Q���擾
    // split: �Ȑ������� (�`��͈�)
    void GetOnUVCurvePoints(const int split, std::vector<Point<double>>& pnts) const
    {
        pnts.clear(); pnts.shrink_to_fit();
        
        std::vector<Vector3d> temp;
        _uv_curve->GetPositionVectors(temp, split);

        // 2�����_�ɕϊ� (GetPositionVectors���\�b�h��3����(z = 0)��ԋp)
        pnts.reserve(temp.size());
        for (auto& pnt : temp)
            pnts.emplace_back(Point<double>(pnt.X, pnt.Y));
    }

    // �G�b�W�𔽓]����
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

// �ʑ��v�f ���[�v
class Loop
{
private:
    std::vector<Edge> _edges;
    int _edge_cnt;

    enum LoopDirection
    {
        CW, // �E���
        CCW, // �����
    };
    LoopDirection _loopDirection;

    // �G�b�W���烋�[�v���\��
    void AdjustForLoop()
    {
        this->MakeLoop();
        this->FixLoop(); // �␳
    }
    // �G�b�W�̌������m���߃��[�v�ɂ���
    // �\�ߗׂ̃G�b�W�����Ԃɕ���ł��邱�Ƃ��K�v
    void MakeLoop()
    {
        const double r = 0.1; // ����_�Ɣ��肷�锻��~�̔��a
        const int split = 1; // 1�Z�O�����g����Ώ\��

        for (int i = 0; i < _edge_cnt; ++i)
        {
            std::vector<Point<double>> edge_pnts; // �G�b�W���\������_�Q
            _edges[i].GetOnUVCurvePoints(split, edge_pnts);
            int edge_pnt_cnt = (int)edge_pnts.size();

            int pre_edge_idx = (i - 1 >= 0) ? i - 1 : _edge_cnt - 1;
            std::vector<Point<double>> pre_edge_pnts; // �O�̃G�b�W���\������_�Q
            _edges[pre_edge_idx].GetOnUVCurvePoints(split, pre_edge_pnts);

            {
                if (_loopDirection == LoopDirection::CCW)
                {
                    // ���̎n�_�ƑO�̃G�b�W�̏I�_�������ł���΃��[�vOK
                    // �����łȂ���΃G�b�W���]
                    if (!IsNear(edge_pnts[0], pre_edge_pnts[1], r))
                        _edges[i].Reverse();
                }
                else
                {
                    throw; // TODO: ����
                }
            }
        }
    }
    // �G�b�W�̒[���q����悤�ɕ␳
    void FixLoop()
    {
        int split = 10; // 1�ł�������

        for (int i = 0; i < _edge_cnt; ++i)
        {
            std::vector<Point<double>> edge_pnts; // �G�b�W���\������_�Q
            _edges[i].GetOnUVCurvePoints(split, edge_pnts);
            int edge_pnt_cnt = (int)edge_pnts.size();

            int next_edge_idx = (i + 1 < _edge_cnt) ? i + 1 : 0;
            std::vector<Point<double>> next_edge_pnts; // ���̃G�b�W���\������_�Q
            _edges[next_edge_idx].GetOnUVCurvePoints(split, next_edge_pnts);

            {
                // �G�b�W�̏I�[�Ǝ��̃G�b�W�̎n�[�̒��_��␳�_�Ƃ���
                double middleX = (edge_pnts[edge_pnt_cnt - 1].x + next_edge_pnts[0].x) / 2;
                double middleY = (edge_pnts[edge_pnt_cnt - 1].y + next_edge_pnts[0].y) / 2;
                Point<double> middle(middleX, middleY);

                edge_pnts[edge_pnt_cnt - 1] = middle; // �I�[
                next_edge_pnts[0] = middle; // �n�[
            }
        }
    }

    // �G�b�W���Ƃ̐܂�_�̒[���q����悤�ɕ␳
    void FixPolyline(std::vector<std::vector<Point<double>>>& polylines) const
    {
        for (int i = 0; i < _edge_cnt; ++i)
        {
            int edge_pnt_cnt = (int)polylines[i].size();
            int next_edge_idx = (i + 1 < _edge_cnt) ? i + 1 : 0;

            // �G�b�W�̏I�[�Ǝ��̃G�b�W�̎n�[�̒��_��␳�_�Ƃ���
            {
                double middleX = (polylines[i][edge_pnt_cnt - 1].x + polylines[next_edge_idx][0].x) / 2;
                double middleY = (polylines[i][edge_pnt_cnt - 1].y + polylines[next_edge_idx][0].y) / 2;
                Point<double> middle(middleX, middleY);

                polylines[i][edge_pnt_cnt - 1] = middle; // �I�[
                polylines[next_edge_idx][0] = middle; // �n�[
            }
        }
    }

public:

    // ���[�vUV���p�`�̖ʐς����߂�
    double GetArea(const int baseline = 0) const
    {
        double area = 0.0; // �ʐ�(�����t)
        const int split = 20; // �G�b�W������

        // �G�b�W����UV�܂���̃��[�v���p�`���쐬����
        std::vector<std::vector<Point<double>>> polylines(_edge_cnt); // �܂���Q
        {
            for (int i = 0; i < _edge_cnt; ++i)
                _edges[i].GetOnUVCurvePoints(split, polylines[i]);

            // �[���Ȃ���悤�ɕ␳
            this->FixPolyline(polylines);
        }

        // ���[�v���p�`�̖ʐς����߂�
        for (int i = 0; i < _edge_cnt; ++i)
        {
            int edge_pnt_cnt = (int)polylines[i].size();

            for (int j = 0; j < edge_pnt_cnt - 1; ++j)
            {
                double height = polylines[i][j].x - polylines[i][j + 1].x;
                double upper = polylines[i][j].y - baseline;
                double lower = polylines[i][j + 1].y - baseline;

                // ��`�ʐ�
                area += (upper + lower) * height * 0.5;
            }

            cout << i + 1 << "�ԃ��[�v�܂ł̖ʐ�: " << area << endl;
        }

        return area;
    }

    Loop(const std::vector<Edge>& edges)
    {
        _edges = edges;
        _edge_cnt = (int)_edges.size();
        _loopDirection = LoopDirection::CCW; // �����Œ�

        this->AdjustForLoop(); // �����t�����s���␳
    }
};
