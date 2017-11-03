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

void Scene::ToggleDrawControlPoints()
{
    auto it = ObjList.begin();

    // 制御点表示トグル
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawCtrlp();
        it++;
    }
}

void Scene::ToggleDrawFirstDiffVectors()
{
    auto it = ObjList.begin();

    // 接線表示トグル
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawFisrtDiff();
        it++;
    }
}

void Scene::ToggleDrawSecondDiffVectors()
{
    auto it = ObjList.begin();

    // 2階微分ベクトル表示トグル
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawSecondDiff();
        it++;
    }
}

void Scene::Draw()
{
    auto it = ObjList.begin();

    // リスト全描画
    while (it != ObjList.end())
    {
        (*it)->Draw(); // モデル描画
        (*it)->DrawControlPointsAndLines(); // 制御点描画
        (*it)->DrawFirstDiffVectors(); // 接線描画
        (*it)->DrawSecondDiffVectors(); // 2階微分ベクトル描画
        (*it)->DrawBox(); // ミニマクスボックス描画
        (*it)->DrawCurvatureVectors(); // 曲率ベクトル描画

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

// マウスピッキング用描画
void Scene::DrawForPick()
{
    auto it = ObjList.begin();

    // リスト全描画
    while (it != ObjList.end())
    {
        glLoadName((*it)->GetObjectNumber());
        (*it)->DrawAsItIs();

        it++;
    }
}

Scene::~Scene()
{
    for (auto it = ObjList.begin(); it != ObjList.end(); it++)
        delete *it;
}