#include "Scene.h"
#include "Axis.h"

Scene::Scene()
{

}

void Scene::AddObject(Object* obj)
{
    ObjList.push_back(obj);
}

void Scene::DeleteObjectAll()
{
    auto it = ObjList.begin();

    while (it != ObjList.end())
    {
        (*it)->RaiseDeleteFlag();

        it++;
    }
}

void Scene::DeleteObjectEnd()
{
    auto it = ObjList.begin();

    if (it == ObjList.end())
        return;

    while (it != ObjList.end())
        it++;

    (*(--it))->RaiseDeleteFlag();
}

void Scene::Draw()
{
    auto size = ObjList.size();
    auto it = ObjList.begin();

    // リスト全描画
    while (it != ObjList.end())
    {
        (*it)->Draw();

        // 削除フラグチェック
        if ((*it)->IsDeleteFlagRaised())
        {
            delete *it;
            (*it) = NULL;
        }

        it++;
    }

    ObjList.remove(NULL);
}

Scene::~Scene()
{
    for (auto it = ObjList.begin(); it != ObjList.end(); it++)
        delete *it;
}