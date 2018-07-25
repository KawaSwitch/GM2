#include "BsplineCurve.h"
#include "ControlPoint.h"
#include <iterator>
#include <cfloat>
#include <memory>
#include <utility>
#include <algorithm>

BsplineCurve::BsplineCurve(const int mord, const ControlPoint* const cp, const int cp_size, const double* const knot,
    const GLdouble* const color, const GLdouble width, const double resol)
{
    _ord = mord;
    _ncpnt = cp_size;
    _nknot = mord + cp_size;

    _min_draw_param = knot[_ord - 1];
    _max_draw_param = knot[_ncpnt];

    SetControlPoint(cp, cp_size);
    SetKnotVector(knot, _nknot);
    SetColor(color);
    _width = width;
    _resolution = resol;

    _length = this->GetLength();
    _draw_vec_length = _length * _draw_vector_ratio;

    // VBO使う
    _isUseVBO = true;
}

// ノットベクトル設定
void BsplineCurve::SetKnotVector(const double* const knot, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("ノットベクトル設定失敗", "knot-vector size must be over 0.");

    // 既に設定されていれば削除
    if (_knot.size() > 0)
      {
	_knot.clear();
	_knot.shrink_to_fit();
      }

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// ノットベクトルをもとにして点群を取得する
// splitSegCnt: セグメントを何分割するかの回数(デフォルトは1 = 分割しない)
vector<Vector3d> BsplineCurve::GetPositionVectorsByKnots(const int splitSegCnt) const
{
    vector<Vector3d> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (unsigned i = 0; i < _knot.size(); ++i)
    {
        // ノットの階数端の非描画部分or重複は省く
      if ((i > 0 && i < (unsigned)_ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(this->GetPositionVector(_knot[i]));

        // 最後は追加しない
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(this->GetPositionVector(_knot[i] + skip * j));
        }
    }

    return pnts;
}
// ノットベクトルをもとにして点群を取得する
// splitSegCnt: セグメントを何分割するかの回数(デフォルトは1 = 分割しない)
vector<Point3dC> BsplineCurve::GetPointsByKnots(const int splitSegCnt) const
{
    vector<Point3dC> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (unsigned i = 0; i < _knot.size(); ++i)
    {
        // ノットの階数端の非描画部分or重複は省く
      if ((i > 0 && i < (unsigned)_ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(Point3dC(this->GetPositionVector(_knot[i]), _knot[i]));

        // 最後は追加しない
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(Point3dC(this->GetPositionVector(_knot[i] + skip * j), _knot[i] + skip * j));
        }
    }

    return pnts;
}

// 事前描画
void BsplineCurve::PreDraw() const
{
    Vector3d pnt;

    glColor4dv(_color);
    glLineWidth(_width);
    glBegin(GL_LINE_STRIP);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        glVertex3d(pnt);
    }

    glEnd();
}

// 頂点バッファ作成
void BsplineCurve::CreateVBO() const
{
    vector<Vector3d> pnts;

    // 頂点取得
    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;
        pnts.push_back(GetPositionVector(t));
    }

    _nVertex_cache = (int)pnts.size();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, pnts.size() * 3 * sizeof(double), (GLdouble*)&pnts[0], GL_DYNAMIC_DRAW);
}

// VBOで描画
void BsplineCurve::DrawVBO() const
{
    glColor4dv(_color);
    glLineWidth(_width);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex_cache);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 接線ベクトル描画
void BsplineCurve::DrawFirstDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetFirstDiffVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 2階微分ベクトル描画
void BsplineCurve::DrawSecondDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetSecondDiffVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 法線ベクトル描画
void BsplineCurve::DrawNormalVectorsInternal() const
{
    Vector3d pnt, normal;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        normal = GetNormalVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + normal);
    }

    glEnd();
}

// 曲率半径描画
void BsplineCurve::DrawCurvatureVectorsInternal() const
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        curv = GetCurvatureVector(t);

        // 曲率半径描画
        glColor4dv(Color::pink);
        glBegin(GL_LINES);
        glVertex3d(pnt);
        glVertex3d(pnt + curv);
        glEnd();

        // 曲率中心描画
        glColor4dv(Color::light_green);
        glBegin(GL_POINTS);
        glVertex3d(pnt + curv);
        glEnd();
    }

    glEnd();
}

// 位置ベクトル取得
Vector3d BsplineCurve::GetPositionVector(const double t) const
{
    Vector3d pnt;

    // 標準的
    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return pnt;
}

// 接線ベクトル取得
Vector3d BsplineCurve::GetFirstDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 2階微分ベクトル取得
Vector3d BsplineCurve::GetSecondDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 通過点から逆変換して曲線を取得する(メンバ関数版)
std::unique_ptr<Curve> BsplineCurve::GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, const GLdouble width) const
{
    return GetBsplineCurveFromPoints(pnts, 4, color, width);
}
// 通観点から逆変換して曲線を取得する
std::unique_ptr<Curve> GetBsplineCurveFromPoints(const vector<Vector3d>& pnts, int ord, const GLdouble* const color, GLdouble width)
{
    int passPntsCnt = (int)pnts.size(); // 通過点数
    int new_ncpnt = (passPntsCnt - 1) + (ord - 1); // 新しい制御点数

    vector<double> new_knots;
    CalcKnotVectorByPassingPnts(pnts, ord, &new_knots);

    vector<ControlPoint> new_cps;
    CalcControlPointsByPassingPnts(pnts, ord, new_knots, &new_cps);

    return std::unique_ptr<BsplineCurve>(new BsplineCurve(ord, &new_cps[0], new_ncpnt, &new_knots[0], color, width));
}

// 参照点からの最近点情報を取得
NearestPointInfoC BsplineCurve::GetNearestPointInfoFromRef(const Vector3d& ref) const
{
    const int seg_split = 8; // セグメント分割数
    auto startPnts = this->GetPointsByKnots(seg_split); // 開始点群

    return Curve::GetNearestPointInfoInternal(ref, startPnts);
}

// ノットの追加
void BsplineCurve::AddKnot(const double t)
{
  unsigned t_i; // ノット挿入に必要な位置
  vector<ControlPoint> new_cps; // 新しい制御点
  vector<double> new_knot; // 新しいノットベクトル
  Vector3d Q;
  
  // 1. 挿入先の決定
  {
    for (unsigned i = 0, s = _knot.size(); i < s - 1; ++i)
      {
	if (_knot[i] <= t && t < _knot[i+1])
	  t_i = i;
      }
    
    // 新しいノットベクトルを作成
    for (unsigned i = 0; i <= t_i; ++i) new_knot.push_back(_knot[i]);
    new_knot.push_back(t);
    for (unsigned i = t_i+1; i < _knot.size(); ++i) new_knot.push_back(_knot[i]);
  }

  // 2. 新制御点の算出
  {
    // 新制御点算出用の比率変数
    auto alpha = [&](int i, int j) -> double
      {
	if (j <= i-_ord +1)
	  return 1;
	else if (i-_ord+2<=j && j <= i)
	  return (t - new_knot[j]) / (new_knot[j+_ord] - new_knot[j]);
	else
	  return 0;
      };

    // 新しい制御点を作成
    new_cps.push_back(_ctrlp[0]);
    for (int j = 1; j < _ncpnt; ++j)
      {
	Q = alpha(t_i, j)*_ctrlp[j] + (1-alpha(t_i, j))*_ctrlp[j-1];
	new_cps.push_back(ControlPoint(Q.X, Q.Y, Q.Z, 1.0));
      }
    new_cps.push_back(_ctrlp[_ncpnt-1]);
  }
  
  // 3. 曲線データの調整
  {
    // 各値設定
    _nknot = new_knot.size();
    SetKnotVector(&(new_knot[0]), new_knot.size());
    
    _ncpnt = new_cps.size();
    SetControlPoint(&(new_cps[0]), new_cps.size());

    // 表示用バッファをすべてクリア
    Object::ClearAllShowingIds();
  }
}

// 2分割した曲線を取得します
std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>>
BsplineCurve::Get2DevidedCurves(const double t)
{
  if (t < _knot[0] || t > _knot[_nknot-1])
    perror("Get2DevidedCurves");
  
  std::shared_ptr<BsplineCurve> left; // 分割曲線の小さいノット側
  std::shared_ptr<BsplineCurve> right; // 分割曲線の大きいノット側
  int add_cnt = 0;
  
  // 分割位置のノット多重度が(階数-1)個になるまでノットを挿入
  {
    int t_cnt_now = std::count(_knot.begin(), _knot.end(), t);

    for (int i = t_cnt_now; i < _ord-1; ++i, ++add_cnt)
      AddKnot(t);
  }

  // 分割位置にノットを足して分割
  {
    vector<double> left_knot, right_knot;
    vector<ControlPoint> left_ctrlp, right_ctrlp;

    // ノット分割
    unsigned ki = 0;
    for (ki = 0; _knot[ki] <= t || fabs(_knot[ki]-t) < EPS::DIFF; ++ki)
      left_knot.push_back(_knot[ki]);
    left_knot.push_back(t); // ノットを足す

    right_knot.push_back(t); // ノットを足す_
    for (ki -= _ord-1; ki < _knot.size(); ++ki)
      right_knot.push_back(_knot[ki]);

    // 制御点分割
    unsigned t_i; // ノット挿入位置
    for (unsigned i = 0, s = _knot.size(); i < s - 1; ++i)
      {
	if (_knot[i] <= t && t < _knot[i+1])
	  t_i = i;
      }

    unsigned ci = 0;
    for (; ci < (t_i + 1) - (_ord - 2) - (_ord - 1)  + (_ord - 2); ++ci)
      left_ctrlp.push_back(_ctrlp[ci]);
    for (--ci; ci < _ctrlp.size(); ++ci)
      right_ctrlp.push_back(_ctrlp[ci]);

    left = std::make_unique<BsplineCurve>(_ord, &(left_ctrlp[0]), left_ctrlp.size(), &(left_knot[0]), Color::red);
    right = std::make_unique<BsplineCurve>(_ord, &(right_ctrlp[0]), right_ctrlp.size(), &(right_knot[0]), Color::blue);
  }
  
  //return std::make_pair(nullptr, nullptr);
  return std::make_pair(std::move(left), std::move(right));
}
