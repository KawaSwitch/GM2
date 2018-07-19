#pragma once

#include "GV.h"
#include <string>
#include <sstream>
#include "Object.h"

// �t�@�C���ǂݍ��݃N���X
class Reader
{
protected:

    string _path; // �f�B���N�g���p�X

public:

    vector<string> GetFilenames(const string& extension = "") const;
    vector<string> ReadAllLines(const string& filepath) const;

    Reader() { _path = ""; }
    Reader(const string& path) { _path = path; }
    ~Reader() {};
};

// �`�󏈗� �Ȑ�/�Ȗʓǂݍ��ݗp�N���X
class KjsReader : public Reader
{
private:

    const string KJS_FILE_NAME = "KJS_FILE"; // .kjs�t�@�C���̊i�[�t�H���_��

    void ExtractNumberFromString(char* dest, const char* src) const;

    Object* BezierCurveReader(const vector<string>& lines) const; // �x�W�G�Ȑ�
    Object* BezierSurfaceReader(const vector<string>& lines) const; // �x�W�G�Ȗ�
    Object* BsplineCurveReader(const vector<string>& lines) const; // B�X�v���C���Ȑ�
    Object* BsplineSurfaceReader(const vector<string>& lines) const; // B�X�v���C���Ȗ�
    Object* NurbsCurveReader(const vector<string>& lines) const; // Nurbs�Ȑ�
    Object* NurbsSurfaceReader(const vector<string>& lines) const; // Nurbs�Ȗ�

public:

    vector<Object *> GetObjectsFromKjsFolder() const;
    Object* GetObjectFromFile(const string& file_name) const;

    using Reader::Reader;
};
