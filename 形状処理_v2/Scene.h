#pragma once

#include "Object.h"
#include <list>
using namespace std;

class Object;

class Scene
{
private:
    list<Object *> ObjList;

public:
    Scene();

    void AddObject(Object *);
    void DeleteObjectAll();
    void DeleteObjectEnd();

    void Draw();
    void DrawForPick();

    ~Scene();
};