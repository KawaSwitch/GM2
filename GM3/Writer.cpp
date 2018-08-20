#include "Writer.h"
#include <fstream>
#include <ctime>

static const int bufSize = 256;

// タイムスタンプを書き出す
void ExportTimeStamp(std::ofstream *log)
{
    char buf[bufSize];

	struct tm now;
	time_t longtime;

	longtime = time(NULL);
	localtime_s(&now, &longtime);
	asctime_s(buf, sizeof(buf), &now);

    *log << buf << std::endl;
}

// 曲線の最近点情報をCSVに書き出す
void ExportCsv_NearestInfoCurve(std::string path, std::vector<NearestPointInfoC> np_infos, bool isNeedTimestamp)
{
    char buf[bufSize];

    std::ofstream log;
    log.open(path, std::ios::trunc); // 同ファイル名があれば上書き

    std::string value = ", 最近点X座標, 最近点Y座標, 最近点Z座標, パラメータ, 点間距離";
    log << value << std::endl;

    int cnt = 0;
    for (const auto &info : np_infos)
    {
        std::string np_data;

        // インデックス
        sprintf_s(buf, bufSize, "%d,", ++cnt);
        np_data += buf;

        // 座標
        sprintf_s(buf, bufSize, "%f,", info.nearestPnt.X);
        np_data += buf;
        sprintf_s(buf, bufSize, "%f,", info.nearestPnt.Y);
        np_data += buf;
		sprintf_s(buf, bufSize, "%f,", info.nearestPnt.Z);
        np_data += buf;

        // パラメータ
		sprintf_s(buf, bufSize, "%f,", info.param);
        np_data += buf;

        // 点間距離
		sprintf_s(buf, bufSize, "%f", info.dist);
        np_data += buf;

        log << np_data << std::endl;
    }

    if (isNeedTimestamp)
        ExportTimeStamp(&log);

    log.close();
}

// 曲面の最近点情報をCSVに書き出す
void ExportCsv_NearestInfoSurface(std::string path, std::vector<NearestPointInfoS> np_infos, bool isNeedTimestamp)
{
    char buf[256];

    std::ofstream log;
    log.open(path, std::ios::trunc); // 同ファイル名があれば上書き

    std::string value = ", 最近点X座標, 最近点Y座標, 最近点Z座標, Uパラメータ, Vパラメータ, 点間距離";
    log << value << std::endl;

    int cnt = 0;
    for (const auto &info : np_infos)
    {
        std::string np_data;

        // インデックス
        sprintf_s(buf, bufSize, "%d,", ++cnt);
        np_data += buf;

        // 座標
        sprintf_s(buf, bufSize, "%f,", info.nearestPnt.X);
        np_data += buf;
        sprintf_s(buf, bufSize, "%f,", info.nearestPnt.Y);
        np_data += buf;
        sprintf_s(buf, bufSize, "%f,", info.nearestPnt.Z);
        np_data += buf;

        // パラメータ
        sprintf_s(buf, bufSize, "%f,", info.paramU);
        np_data += buf;
        sprintf_s(buf, bufSize, "%f,", info.paramV);
        np_data += buf;

        // 点間距離
        sprintf_s(buf, bufSize, "%f", info.dist);
        np_data += buf;

        log << np_data << std::endl;
    }

    if (isNeedTimestamp)
        ExportTimeStamp(&log);

    log.close();
}
