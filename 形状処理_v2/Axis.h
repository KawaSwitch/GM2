#pragma once

#include "GV.h"
#include "Object.h"

class Axis
{
protected:
    double _length; // 胴体部分の長さ

public:
    Axis(double len) { _length = len; }

    // 矢印部分を含めた長さを返す
    double GetLength() { return _length + _length / 4; }
};

// Cggl風の軸
class NormalAxis : public Axis
{
private:
    void DrawInternal();

public:
    NormalAxis(double len) : Axis(len) { }
    void Draw();
};