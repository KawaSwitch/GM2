#include "Reader.h"
#include "Model.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <cstring>

// �w��g���q�̃t�@�C���������ׂĎ擾
vector<string> Reader::GetFilenames(const string& extension) const
{
    vector<string> returnFiles;

    DIR *dp;       // �f�B���N�g���|�C���^
    dirent* entry; // readdir() �ŕԂ����G���g���[�|�C���g

    string path = (_path == "") ? "./" : _path; // �p�X�w�肪�Ȃ���΃J�����g
    string ext = "." + extension;

    dp = opendir(path.c_str());

    if (dp == NULL)
    {
        Error::ShowMessage(
            "�t�@�C���ǂݍ��݂Ɏ��s���܂���.",
            "�t�@�C���̃f�B���N�g�����Q�Ƃł��܂���.");
    }

    do
    {
        entry = readdir(dp);

        if (entry != NULL)
        {
            if (ext.size() == 1) // �g���q�w��Ȃ�
                returnFiles.push_back(entry->d_name);
            else
            {
                string filename(entry->d_name);
                if (filename.size() < ext.size())
                    continue;

                string buf = filename.substr(strlen(entry->d_name) - ext.size(), ext.size());

                if (buf == ext) // �g���q�`�F�b�N
                    returnFiles.push_back(filename);
            }
        }
    } while (entry != NULL);

    closedir(dp);

    return returnFiles;
}

// 1�s�����ׂēǂݍ���ŕԂ�
vector<string> Reader::ReadAllLines(const string& filepath) const
{
    vector<string> returnLines;
    std::ifstream ifs(filepath);
    string str;

    if (ifs.fail())
    {
        Error::ShowMessage(
            "�t�@�C���ǂݍ��݂Ɏ��s���܂���.",
            "�t�@�C�����Q�Ƃł��܂���.");
    }

    while (getline(ifs, str))
        returnLines.push_back(str);

    return returnLines;
}

// �t�@�C������I�u�W�F�N�g���擾����
Object* KjsReader::GetObjectFromFile(const string& file_name) const
{
    string file_path = KJS_FILE_NAME + "/" + file_name;

    //// @������Ώ���
    //for (size_t c = file_path.find_first_of("@"); c != string::npos; c = c = file_path.find_first_of("@"))
    //    file_path.erase(c, 1);

    std::ifstream ifs(file_path);
    bool canOpen = ifs.is_open();

    //// @�t��������
    //if (!canOpen)
    //{
    //    file_path = KJS_FILE_NAME + "/" + "@" + file_name;
    //    ifs.clear();
    //    ifs(file_path);
    //    canOpen = ifs.is_open();
    //}

    if (!canOpen)
    {
        auto detail = file_path + "�Ƃ����t�@�C���͑��݂��܂���.";
        Error::ShowMessage("�t�@�C���ǂݍ��݂Ɏ��s���܂���");
        return NULL;
        //Error::ShowAndExit(
        //    "�t�@�C���ǂݍ��݂Ɏ��s���܂���.",
        //    detail.c_str());
    }

    auto lines = ReadAllLines(file_path);

    if (lines.size() < 5) // ���炩�ɂ��������Ƃ�
        return nullptr;

    // �啶���ϊ�
    transform(lines[0].begin(), lines[0].end(), lines[0].begin(), ::toupper);
    transform(lines[1].begin(), lines[1].end(), lines[1].begin(), ::toupper);
    
    if (strstr(lines[0].c_str(), "BEZIER"))
    {
      if (strstr(lines[1].c_str(), "CURVE"))
	return BezierCurveReader(lines);
      else
	return BezierSurfaceReader(lines);
    }
    else if (strstr(lines[0].c_str(), "BSPLINE"))
    {
      if (strstr(lines[1].c_str(), "CURVE"))
	return BsplineCurveReader(lines);
      else
	return BsplineSurfaceReader(lines);
    }
    else if (strstr(lines[0].c_str(), "NURBS"))
    {
      if (strstr(lines[1].c_str(), "CURVE"))
	return NurbsCurveReader(lines);
      else
	return NurbsSurfaceReader(lines);
    }
    else
    {
        Error::ShowMessage("kjs�t�@�C���ǂݍ��ݎ��s");
        return nullptr;
    }
}

// KJS�t�H���_���̐擪��@�̂���.kjs�t�@�C�����ׂĂ���I�u�W�F�N�g���擾����
vector<Object *> KjsReader::GetObjectsFromKjsFolder() const
{
    vector<Object *> returnObjs;

    // kjs�g���q�t�@�C�����擾
    auto kjsFiles = GetFilenames("kjs");

    for (int i = 0; i < kjsFiles.size(); i++)
    {
        // �t�@�C�����̐擪�� @ �̃t�@�C���̂ݓǂݍ���!
        if (kjsFiles[i].front() != '@')
            continue;

        // �t�@�C������I�u�W�F�N�g���擾����
        Object* obj = GetObjectFromFile(kjsFiles[i]);
        if (obj != NULL)
            returnObjs.push_back(GetObjectFromFile(kjsFiles[i]));
    }

    return returnObjs;
}

// ������̒����琔���݂̂����o��
void KjsReader::ExtractNumberFromString(char* dest, const char* src) const
{
    while (*src)
    {
        if ('0' <= *src && *src <= '9')
            *dest++ = *src;

        src++;
        *dest = 0;
    }
}

// �ȉ��e�I�u�W�F�N�g�̃��[�_�[(�Q�b�^�[)�֐�
// ���X�����̂ł܂Ƃ߂ꂽ��܂Ƃ߂���
Object* KjsReader::BezierCurveReader(const vector<string>& lines) const
{
    int current = 2; // 3�s�ڂ���ǂݍ���
    char mord[8]; // �K��
    char ncpnt[8]; // ����_��
    GLdouble color[4]; // �F
    GLdouble width; // ��

    // �F
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // ��
    {
        stringstream ss(lines[current++]);
        ss >> width;
    }

    // �e�v���p�e�B�擾
    ExtractNumberFromString(mord, lines[current++].c_str());
    ExtractNumberFromString(ncpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ncpnt));
    current++; // CONTROL POINT�錾

    for (int i = 0; i < cps.size(); i++)
    {
        double x, y, z;
        double buf[8];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> x >> y >> z;

        ControlPoint cp(x, y, z);
        cps[i] = cp;
    }

    BezierCurve* curve = new BezierCurve(atoi(mord), &cps[0], atoi(ncpnt), color, width);
    return curve;
}
Object* KjsReader::BezierSurfaceReader(const vector<string>& lines) const
{
    int current = 2; // 3�s�ڂ���ǂݍ���
    char uord[8]; // u�K��
    char ucpnt[8]; // u����_��
    char vord[8]; // v�K��
    char vcpnt[8]; // v����_��
    GLdouble color[4]; // �F
    GLdouble resol; // �𑜓x

    // �F
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // �𑜓x
    {
        stringstream ss(lines[current++]);
        ss >> resol;
    }

    // �e�v���p�e�B�擾
    ExtractNumberFromString(uord, lines[current++].c_str());
    ExtractNumberFromString(ucpnt, lines[current++].c_str());
    ExtractNumberFromString(vord, lines[current++].c_str());
    ExtractNumberFromString(vcpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ucpnt) * atoi(vcpnt));
    current++; // CONTROL POINT�錾

    for (int i = 0; i < cps.size(); i++)
    {
        double x, y, z;
        double buf[8];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> buf[1] >> x >> y >> z;

        ControlPoint cp(x, y, z);
        cps[i] = cp;
    }

    BezierSurface* surf = new BezierSurface(atoi(uord), atoi(vord), &cps[0], atoi(ucpnt), atoi(vcpnt), color, resol);
    return surf;
}
Object* KjsReader::BsplineCurveReader(const vector<string>& lines) const
{
    int current = 2; // 3�s�ڂ���ǂݍ���
    char mord[8]; // �K��
    char ncpnt[8]; // ����_��
    GLdouble color[4]; // �F
    GLdouble width; // ��

    // �F
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // ��
    {
        stringstream ss(lines[current++]);
        ss >> width;
    }

    // �e�v���p�e�B�擾
    ExtractNumberFromString(mord, lines[current++].c_str());
    ExtractNumberFromString(ncpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ncpnt));
    current++; // CONTROL POINT�錾

    // ����_�擾
    for (int i = 0; i < cps.size(); i++)
    {
        double x, y, z;
        double buf[8];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> x >> y >> z;



        ControlPoint cp(x, y, z);
        cps[i] = cp;
    }

    vector<double> knot;
    knot.resize(atoi(mord) + atoi(ncpnt));
    current++; // KNOTS�錾

    // �m�b�g��擾
    for (int i = 0; i < knot.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knot[i];
    }

    BsplineCurve* curve = new BsplineCurve(atoi(mord), &cps[0], atoi(ncpnt), &knot[0], color, width);
    return curve;
}
Object* KjsReader::BsplineSurfaceReader(const vector<string>& lines) const
{
    int current = 2; // 3�s�ڂ���ǂݍ���
    char uord[8]; // u�K��
    char ucpnt[8]; // u����_��
    char vord[8]; // v�K��
    char vcpnt[8]; // v����_��
    GLdouble color[4]; // �F
    GLdouble resol; // �𑜓x

    // �F
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // �𑜓x
    {
        stringstream ss(lines[current++]);
        ss >> resol;
    }

    // �e�v���p�e�B�擾
    ExtractNumberFromString(uord, lines[current++].c_str());
    ExtractNumberFromString(ucpnt, lines[current++].c_str());
    ExtractNumberFromString(vord, lines[current++].c_str());
    ExtractNumberFromString(vcpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ucpnt) * atoi(vcpnt));
    current++; // CONTROL POINT�錾

    // ����_�擾
    for (int i = 0; i < cps.size(); i++)
    {
        double x, y, z;
        int row, col;

        stringstream ss(lines[current++]);
        ss >> row >> col >> x >> y >> z;

        ControlPoint cp(x, y, z);
        cps[i] = cp;
    }

    vector<double> knotU;
    knotU.resize(atoi(uord) + atoi(ucpnt));
    current++; // U KNOTS�錾

    // U�m�b�g��擾
    for (int i = 0; i < knotU.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knotU[i];
    }

    vector<double> knotV;
    knotV.resize(atoi(vord) + atoi(vcpnt));
    current++; // V KNOTS�錾

    // V�m�b�g��擾
    for (int i = 0; i < knotV.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knotV[i];
    }

    BsplineSurface* surf = 
        new BsplineSurface(atoi(uord), atoi(vord), &cps[0], atoi(ucpnt), atoi(vcpnt), &knotU[0], &knotV[0], color, resol);

    return surf;
}
Object* KjsReader::NurbsCurveReader(const vector<string>& lines) const
{
    int current = 2; // 3�s�ڂ���ǂݍ���
    char mord[8]; // �K��
    char ncpnt[8]; // ����_��
    GLdouble color[4]; // �F
    GLdouble width; // ��

    // �F
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // ��
    {
        stringstream ss(lines[current++]);
        ss >> width;
    }

    // �e�v���p�e�B�擾
    ExtractNumberFromString(mord, lines[current++].c_str());
    ExtractNumberFromString(ncpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ncpnt));
    current++; // CONTROL POINT�錾

    // ����_�擾
    for (int i = 0; i < cps.size(); i++)
    {
        double x, y, z, w;
        double buf[8];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> x >> y >> z >> w;

        ControlPoint cp(x, y, z, w);
        cps[i] = cp;
    }

    vector<double> knot;
    knot.resize(atoi(mord) + atoi(ncpnt));
    current++; // KNOTS�錾

    // �m�b�g��擾
    for (int i = 0; i < knot.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knot[i];
    }

    NurbsCurve* curve = new NurbsCurve(atoi(mord), &cps[0], atoi(ncpnt), &knot[0], color, width);
    return curve;
}
Object* KjsReader::NurbsSurfaceReader(const vector<string>& lines) const
{
    int current = 2; // 3�s�ڂ���ǂݍ���
    char uord[8]; // u�K��
    char ucpnt[8]; // u����_��
    char vord[8]; // v�K��
    char vcpnt[8]; // v����_��
    GLdouble color[4]; // �F
    GLdouble resol; // �𑜓x

    // �F
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // �𑜓x
    {
        stringstream ss(lines[current++]);
        ss >> resol;
    }

    // �e�v���p�e�B�擾
    ExtractNumberFromString(uord, lines[current++].c_str());
    ExtractNumberFromString(ucpnt, lines[current++].c_str());
    ExtractNumberFromString(vord, lines[current++].c_str());
    ExtractNumberFromString(vcpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ucpnt) * atoi(vcpnt));
    current++; // CONTROL POINT�錾

    // ����_�擾
    for (int i = 0; i < cps.size(); i++)
    {
        double x, y, z, w;
        int row, col;

        stringstream ss(lines[current++]);
        ss >> row >> col >> x >> y >> z >> w;

        ControlPoint cp(x, y, z, w);
        cps[i] = cp;
    }

    vector<double> knotU;
    knotU.resize(atoi(uord) + atoi(ucpnt));
    current++; // U KNOTS�錾

    // U�m�b�g��擾
    for (int i = 0; i < knotU.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knotU[i];
    }

    vector<double> knotV;
    knotV.resize(atoi(vord) + atoi(vcpnt));
    current++; // V KNOTS�錾

    // V�m�b�g��擾
    for (int i = 0; i < knotV.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knotV[i];
    }

    NurbsSurface* surf =
        new NurbsSurface(atoi(uord), atoi(vord), &cps[0], atoi(ucpnt), atoi(vcpnt), &knotU[0], &knotV[0], color, resol);

    return surf;
}
