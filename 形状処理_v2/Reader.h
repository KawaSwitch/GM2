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

    vector<string> GetFilenames(string extension = "");
    vector<string> ReadAllLines(string filepath);

    Reader() { _path = ""; }
    Reader(string path) { _path = path; }
    ~Reader() {};
};

// 形状処理 曲線/曲面読み込み用クラス
class KjsReader : public Reader
{
private:

    string KJS_FILE_NAME = "KJS_FILE"; // .kjsファイルの格納フォルダ名

    Object* BezierCurveReader(vector<string> lines); // ベジエ曲線
    Object* BezierSurfaceReader(vector<string> lines); // ベジエ曲面
    Object* BsplineCurveReader(vector<string> lines); // Bスプライン曲線
    Object* BsplineSurfaceReader(vector<string> lines); // Bスプライン曲面

public:

    vector<Object *> GetObjectsFromKjsFolder();
    Object* GetObjectFromFile(string file_name);

    using Reader::Reader;
};
