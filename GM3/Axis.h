#pragma once

#include "GV.h"
#include "Object.h"

// 軸の表示タイプ
enum AxisShowType
{
    None,      // 表示しない
    Normal,    // 普通に描画
    WithLabel, // XYZラベルも描画
};

extern AxisShowType axisShowType;

class Axis
{
  private:
    // 矢印部分の占める割合
    const double _ratio = (double)1 / 3;

  protected:
    double _length;
    double _bodyLength;  // 胴体部分の長さ
    double _arrowLength; // 矢印部分の長さ

  public:
    Axis(double len)
    {
        _length = len;

        _bodyLength = len * (1 - _ratio);
        _arrowLength = len * _ratio;
    }

    // 矢印部分を含めた長さを返す
    double GetLength() { return _length; }
};

// ソリッド軸
class NormalAxis : public Axis
{
  private:
    void DrawInternal();
    void DrawCharacter();

  public:
    NormalAxis(double len) : Axis(len) {}
    void Draw();
};
