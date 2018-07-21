#include "Writer.h"
#include <fstream>
#include <ctime>
#include <error.h>
#include <time.h>

// �^�C���X�^���v�������o��
void ExportTimeStamp(std::ofstream* log)
{
    char buf[256];

    time_t longtime = time(NULL);
    struct tm now;

    localtime_r(&longtime, &now);
    asctime_r(&now, buf);

    *log << buf << std::endl;
}

// �Ȑ��̍ŋߓ_����CSV�ɏ����o��
void ExportCsv_NearestInfoCurve(std::string path, std::vector<NearestPointInfoC> np_infos, bool isNeedTimestamp)
{
    char buf[256];

    std::ofstream log;
    log.open(path, std::ios::trunc); // ���t�@�C����������Ώ㏑��

    std::string value = ", �ŋߓ_X���W, �ŋߓ_Y���W, �ŋߓ_Z���W, �p�����[�^, �_�ԋ���";
    log << value << std::endl;
    
    int cnt = 0;
    for (const auto& info : np_infos)
    {
        std::string np_data;

        // �C���f�b�N�X
        sprintf(buf, "%d,", ++cnt);
        np_data += buf;

        // ���W
        sprintf(buf, "%f,", info.nearestPnt.X);
        np_data += buf;
        sprintf(buf, "%f,", info.nearestPnt.Y);
        np_data += buf;
        sprintf(buf, "%f,", info.nearestPnt.Z);
        np_data += buf;

        // �p�����[�^
        sprintf(buf, "%f,", info.param);
        np_data += buf;

        // �_�ԋ���
        sprintf(buf, "%f", info.dist);
        np_data += buf;

        log << np_data << std::endl;
    }

    if (isNeedTimestamp)
        ExportTimeStamp(&log);

    log.close();
}

// �Ȗʂ̍ŋߓ_����CSV�ɏ����o��
void ExportCsv_NearestInfoSurface(std::string path, std::vector<NearestPointInfoS> np_infos, bool isNeedTimestamp)
{
    char buf[256];

    std::ofstream log;
    log.open(path, std::ios::trunc); // ���t�@�C����������Ώ㏑��

    std::string value = ", �ŋߓ_X���W, �ŋߓ_Y���W, �ŋߓ_Z���W, U�p�����[�^, V�p�����[�^, �_�ԋ���";
    log << value << std::endl;

    int cnt = 0;
    for (const auto& info : np_infos)
    {
        std::string np_data;

        // �C���f�b�N�X
        sprintf(buf, "%d,", ++cnt);
        np_data += buf;

        // ���W
        sprintf(buf, "%f,", info.nearestPnt.X);
        np_data += buf;
        sprintf(buf, "%f,", info.nearestPnt.Y);
        np_data += buf;
        sprintf(buf, "%f,", info.nearestPnt.Z);
        np_data += buf;

        // �p�����[�^
        sprintf(buf, "%f,", info.paramU);
        np_data += buf;
        sprintf(buf, "%f,", info.paramV);
        np_data += buf;

        // �_�ԋ���
        sprintf(buf, "%f", info.dist);
        np_data += buf;

        log << np_data << std::endl;
    }

    if (isNeedTimestamp)
        ExportTimeStamp(&log);

    log.close();
}
