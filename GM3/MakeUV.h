#pragma once

#include <vector>
#include <memory>
#include "Model.h"

// UV�Ȑ�
std::shared_ptr<BsplineCurve> GetOnSurfaceUVParams(const std::shared_ptr<BsplineCurve>& curve, const std::shared_ptr<BsplineSurface>& surf,
    std::vector<Point<double>>& uv_params);

//void GetOnSurfaceUVCurve()