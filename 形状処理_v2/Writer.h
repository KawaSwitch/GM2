#pragma once

#include <string>
#include <vector>
#include "NearestPointInfo.h"

void ExportCsv_NearestInfoCurve(std::string path, std::vector<NearestPointInfoC> np_infos, bool isNeedTimestamp = false);