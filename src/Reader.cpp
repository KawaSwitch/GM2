#include "Reader.h"
#include "Model.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <cstring>

// 指定拡張子のファイル名をすべて取得
vector<string> Reader::GetFilenames(const string& extension) const
{
    vector<string> returnFiles;

    DIR *dp;       // ディレクトリポインタ
    dirent* entry; // readdir() で返されるエントリーポイント

    string path = (_path == "") ? "./" : _path; // パス指定がなければカレント
    string ext = "." + extension;

    dp = opendir(path.c_str());

    if (dp == NULL)
    {
        Error::ShowMessage(
            "ファイル読み込みに失敗しました.",
            "ファイルのディレクトリが参照できません.");
    }

    do
    {
        entry = readdir(dp);

        if (entry != NULL)
        {
            if (ext.size() == 1) // 拡張子指定なし
                returnFiles.push_back(entry->d_name);
            else
            {
                string filename(entry->d_name);
                if (filename.size() < ext.size())
                    continue;

                string buf = filename.substr(strlen(entry->d_name) - ext.size(), ext.size());

                if (buf == ext) // 拡張子チェック
                    returnFiles.push_back(filename);
            }
        }
    } while (entry != NULL);

    closedir(dp);

    return returnFiles;
}

// 1行ずつすべて読み込んで返す
vector<string> Reader::ReadAllLines(const string& filepath) const
{
    vector<string> returnLines;
    std::ifstream ifs(filepath);
    string str;

    if (ifs.fail())
    {
        Error::ShowMessage(
            "ファイル読み込みに失敗しました.",
            "ファイルが参照できません.");
    }

    while (getline(ifs, str))
        returnLines.push_back(str);

    return returnLines;
}

// ファイルからオブジェクトを取得する
Object* KjsReader::GetObjectFromFile(const string& file_name) const
{
    string file_path = KJS_FILE_NAME + "/" + file_name;

    //// @があれば消去
    //for (size_t c = file_path.find_first_of("@"); c != string::npos; c = c = file_path.find_first_of("@"))
    //    file_path.erase(c, 1);

    std::ifstream ifs(file_path);
    bool canOpen = ifs.is_open();

    //// @付きも試す
    //if (!canOpen)
    //{
    //    file_path = KJS_FILE_NAME + "/" + "@" + file_name;
    //    ifs.clear();
    //    ifs(file_path);
    //    canOpen = ifs.is_open();
    //}

    if (!canOpen)
    {
        auto detail = file_path + "というファイルは存在しません.";
        Error::ShowMessage("ファイル読み込みに失敗しました");
        return NULL;
        //Error::ShowAndExit(
        //    "ファイル読み込みに失敗しました.",
        //    detail.c_str());
    }

    auto lines = ReadAllLines(file_path);

    if (lines.size() < 5) // 明らかにおかしいとき
        return nullptr;

    // 大文字変換
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
        Error::ShowMessage("kjsファイル読み込み失敗");
        return nullptr;
    }
}

// KJSフォルダ内の先頭に@のついた.kjsファイルすべてからオブジェクトを取得する
vector<Object *> KjsReader::GetObjectsFromKjsFolder() const
{
    vector<Object *> returnObjs;

    // kjs拡張子ファイルを取得
    auto kjsFiles = GetFilenames("kjs");

    for (int i = 0; i < kjsFiles.size(); i++)
    {
        // ファイル名の先頭が @ のファイルのみ読み込む!
        if (kjsFiles[i].front() != '@')
            continue;

        // ファイルからオブジェクトを取得する
        Object* obj = GetObjectFromFile(kjsFiles[i]);
        if (obj != NULL)
            returnObjs.push_back(GetObjectFromFile(kjsFiles[i]));
    }

    return returnObjs;
}

// 文字列の中から数字のみを取り出す
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

// 以下各オブジェクトのリーダー(ゲッター)関数
// 長々しいのでまとめれたらまとめたい
Object* KjsReader::BezierCurveReader(const vector<string>& lines) const
{
    int current = 2; // 3行目から読み込む
    char mord[8]; // 階数
    char ncpnt[8]; // 制御点数
    GLdouble color[4]; // 色
    GLdouble width; // 幅

    // 色
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // 幅
    {
        stringstream ss(lines[current++]);
        ss >> width;
    }

    // 各プロパティ取得
    ExtractNumberFromString(mord, lines[current++].c_str());
    ExtractNumberFromString(ncpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ncpnt));
    current++; // CONTROL POINT宣言

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
    int current = 2; // 3行目から読み込む
    char uord[8]; // u階数
    char ucpnt[8]; // u制御点数
    char vord[8]; // v階数
    char vcpnt[8]; // v制御点数
    GLdouble color[4]; // 色
    GLdouble resol; // 解像度

    // 色
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // 解像度
    {
        stringstream ss(lines[current++]);
        ss >> resol;
    }

    // 各プロパティ取得
    ExtractNumberFromString(uord, lines[current++].c_str());
    ExtractNumberFromString(ucpnt, lines[current++].c_str());
    ExtractNumberFromString(vord, lines[current++].c_str());
    ExtractNumberFromString(vcpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ucpnt) * atoi(vcpnt));
    current++; // CONTROL POINT宣言

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
    int current = 2; // 3行目から読み込む
    char mord[8]; // 階数
    char ncpnt[8]; // 制御点数
    GLdouble color[4]; // 色
    GLdouble width; // 幅

    // 色
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // 幅
    {
        stringstream ss(lines[current++]);
        ss >> width;
    }

    // 各プロパティ取得
    ExtractNumberFromString(mord, lines[current++].c_str());
    ExtractNumberFromString(ncpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ncpnt));
    current++; // CONTROL POINT宣言

    // 制御点取得
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
    current++; // KNOTS宣言

    // ノット列取得
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
    int current = 2; // 3行目から読み込む
    char uord[8]; // u階数
    char ucpnt[8]; // u制御点数
    char vord[8]; // v階数
    char vcpnt[8]; // v制御点数
    GLdouble color[4]; // 色
    GLdouble resol; // 解像度

    // 色
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // 解像度
    {
        stringstream ss(lines[current++]);
        ss >> resol;
    }

    // 各プロパティ取得
    ExtractNumberFromString(uord, lines[current++].c_str());
    ExtractNumberFromString(ucpnt, lines[current++].c_str());
    ExtractNumberFromString(vord, lines[current++].c_str());
    ExtractNumberFromString(vcpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ucpnt) * atoi(vcpnt));
    current++; // CONTROL POINT宣言

    // 制御点取得
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
    current++; // U KNOTS宣言

    // Uノット列取得
    for (int i = 0; i < knotU.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knotU[i];
    }

    vector<double> knotV;
    knotV.resize(atoi(vord) + atoi(vcpnt));
    current++; // V KNOTS宣言

    // Vノット列取得
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
    int current = 2; // 3行目から読み込む
    char mord[8]; // 階数
    char ncpnt[8]; // 制御点数
    GLdouble color[4]; // 色
    GLdouble width; // 幅

    // 色
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // 幅
    {
        stringstream ss(lines[current++]);
        ss >> width;
    }

    // 各プロパティ取得
    ExtractNumberFromString(mord, lines[current++].c_str());
    ExtractNumberFromString(ncpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ncpnt));
    current++; // CONTROL POINT宣言

    // 制御点取得
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
    current++; // KNOTS宣言

    // ノット列取得
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
    int current = 2; // 3行目から読み込む
    char uord[8]; // u階数
    char ucpnt[8]; // u制御点数
    char vord[8]; // v階数
    char vcpnt[8]; // v制御点数
    GLdouble color[4]; // 色
    GLdouble resol; // 解像度

    // 色
    {
        stringstream ss(lines[current++]);
        ss >> color[0] >> color[1] >> color[2] >> color[3];
    }

    // 解像度
    {
        stringstream ss(lines[current++]);
        ss >> resol;
    }

    // 各プロパティ取得
    ExtractNumberFromString(uord, lines[current++].c_str());
    ExtractNumberFromString(ucpnt, lines[current++].c_str());
    ExtractNumberFromString(vord, lines[current++].c_str());
    ExtractNumberFromString(vcpnt, lines[current++].c_str());

    vector<ControlPoint> cps;
    cps.resize(atoi(ucpnt) * atoi(vcpnt));
    current++; // CONTROL POINT宣言

    // 制御点取得
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
    current++; // U KNOTS宣言

    // Uノット列取得
    for (int i = 0; i < knotU.size(); i++)
    {
        double buf[4];

        stringstream ss(lines[current++]);
        ss >> buf[0] >> knotU[i];
    }

    vector<double> knotV;
    knotV.resize(atoi(vord) + atoi(vcpnt));
    current++; // V KNOTS宣言

    // Vノット列取得
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
