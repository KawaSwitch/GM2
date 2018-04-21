#pragma once

#include "GV.h"
#include <string>
#include <sstream>
#include "Object.h"

// ファイル読み込みクラス
class Reader
{
protected:

    string _path; // ディレクトリパス

public:

    vector<string> GetFilenames(const string& extension = "") const;
    vector<string> ReadAllLines(const string& filepath) const;

    Reader() { _path = ""; }
    Reader(const string& path) { _path = path; }
    ~Reader() {};
};

// 形状処理 曲線/曲面読み込み用クラス
class KjsReader : public Reader
{
private:

    const string KJS_FILE_NAME = "KJS_FILE"; // .kjsファイルの格納フォルダ名

    void ExtractNumberFromString(char* dest, const char* src) const;

    Object* BezierCurveReader(const vector<string>& lines) const; // ベジエ曲線
    Object* BezierSurfaceReader(const vector<string>& lines) const; // ベジエ曲面
    Object* BsplineCurveReader(const vector<string>& lines) const; // Bスプライン曲線
    Object* BsplineSurfaceReader(const vector<string>& lines) const; // Bスプライン曲面
    Object* NurbsCurveReader(const vector<string>& lines) const; // Nurbs曲線
    Object* NurbsSurfaceReader(const vector<string>& lines) const; // Nurbs曲面

public:

    vector<Object *> GetObjectsFromKjsFolder() const;
    Object* GetObjectFromFile(const string& file_name) const;

    using Reader::Reader;
};
