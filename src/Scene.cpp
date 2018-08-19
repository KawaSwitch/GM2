#include "Scene.h"
#include "Axis.h"
#include <memory>

Scene::Scene()
{
}

void Scene::AddObject(std::string key, shared_ptr<Object> obj)
{
    _objTable.emplace(key, obj);
}

void Scene::DeleteObjectAll()
{
    auto it = _objTable.begin();

    while (it != _objTable.end())
    {
        (*it).second->RaiseDeleteFlag();
        it++;
    }
}

void Scene::DeleteObjectEnd()
{
    auto it = _objTable.begin();

    if (it == _objTable.end())
        return;

    while (it != _objTable.end())
        it++;

    (*(--it)).second->RaiseDeleteFlag();
}

void Scene::ToggleDrawControlPoints()
{
    auto it = _objTable.begin();

    // 制御点表示トグル
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawCtrlp();
        it++;
    }
}

void Scene::ToggleDrawFirstDiffVectors()
{
    auto it = _objTable.begin();

    // 接線表示トグル
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawFisrtDiff();
        it++;
    }
}

void Scene::ToggleDrawSecondDiffVectors()
{
    auto it = _objTable.begin();

    // 2階微分ベクトル表示トグル
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawSecondDiff();
        it++;
    }
}

void Scene::ToggleDrawBox()
{
    auto it = _objTable.begin();

    // ミニマクスボックス表示トグル
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawBox();
        it++;
    }
}

void Scene::ToggleDrawNormalVectors()
{
    auto it = _objTable.begin();

    // 法線表示トグル
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawNormal();
        it++;
    }
}

void Scene::ToggleDrawCurvatureVectors()
{
    auto it = _objTable.begin();

    // 曲率ベクトル表示トグル
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawCurvature();
        it++;
    }
}

void Scene::Draw()
{
    // α値がある場合を考慮して最後に描く
    // 形状情報描画(非透明)
    for (const auto &obj : _objTable)
    {
        obj.second->DrawControlPointsAndLines(); // 制御点描画
        obj.second->DrawFirstDiffVectors();      // 接線描画
        obj.second->DrawSecondDiffVectors();     // 2階微分ベクトル描画
        obj.second->DrawBox();                   // ミニマクスボックス描画
        obj.second->DrawNormalVectors();         // 法線描画
        obj.second->DrawCurvatureVectors();      // 曲率ベクトル描画
    }
    // 形状描画
    // 形状同士のαでの順番考慮
    for (const auto &obj : _objTable)
    {
        if (!obj.second->IsSemiTransparent())
            obj.second->Draw(); // モデル描画
    }
    for (const auto &obj : _objTable)
    {
        if (obj.second->IsSemiTransparent())
            obj.second->Draw(); // モデル描画
    }

    // for (auto it = ObjList.begin(), end = ObjList.end(); it != end; it++)
    // {
    //     // 削除フラグチェック
    //     if ((*it)->IsDeleteFlagRaised())
    //     {
    //         delete *it;
    //         (*it) = NULL;
    //     }
    // }

    // ObjList.remove(NULL);
}

// マウスピッキング用描画
void Scene::DrawForPick()
{
    auto it = _objTable.begin();

    // リスト全描画
    while (it != _objTable.end())
    {
        glLoadName((*it).second->GetObjectNumber());
        (*it).second->DrawAsItIs();

        it++;
    }
}

// すべてのMMボックスを囲むボックスを取得する
Box Scene::GetCoverBound()
{
    vector<Box> allBound;

    for (auto it = _objTable.begin(); it != _objTable.end(); ++it)
        allBound.push_back((*it).second->GetBound());

    return Box(allBound);
}

Scene::~Scene()
{
}
