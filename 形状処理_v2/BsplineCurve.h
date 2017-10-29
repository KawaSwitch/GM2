#pragma once

#include "GV.h"
#include "Object.h"
#include "Curve.h"

// Bスプライン曲線
class BsplineCurve : public Curve
{
private:

    // 事前描画
    virtual void PreDraw() override;

public:


};