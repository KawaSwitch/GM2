#pragma once

#include <string>
#include <vector>
#include "NearestPointInfo.h"

void ExportTimeStamp(std::ofstream *log);
void ExportCsv_NearestInfoCurve(std::string path, std::vector<NearestPointInfoC> np_infos, bool isNeedTimestamp = false);
void ExportCsv_NearestInfoSurface(std::string path, std::vector<NearestPointInfoS> np_infos, bool isNeedTimestamp = false);
