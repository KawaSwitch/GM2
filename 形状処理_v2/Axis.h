#pragma once

#include "GV.h"
#include "Object.h"

// 軸の表示タイプ
enum AxisShowType
{
    None,  // 表示しない
    Normal, // 普通に描画
    WithLabel, // XYZラベルも描画
};

extern AxisShowType axisShowType;

class Axis
{
protected:
    double _length; // 胴体部分の長さ

public:
    Axis(double len) { _length = len; }

    // 矢印部分を含めた長さを返す
    double GetLength() { return _length + _length / 4; }
};

// ソリッド軸
class NormalAxis : public Axis
{
private:
    void DrawInternal();
    void DrawCharacter();

public:
    NormalAxis(double len) : Axis(len) { }
    void Draw();
};