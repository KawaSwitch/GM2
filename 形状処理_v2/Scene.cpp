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

void Scene::ToggleDrawBox()
{
    auto it = ObjList.begin();

    // ミニマクスボックス表示トグル
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawBox();
        it++;
    }
}

void Scene::ToggleDrawNormalVectors()
{
    auto it = ObjList.begin();

    // 法線表示トグル
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawNormal();
        it++;
    }
}

void Scene::ToggleDrawCurvatureVectors()
{
    auto it = ObjList.begin();

    // 曲率ベクトル表示トグル
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawCurvature();
        it++;
    }
}

void Scene::Draw()
{
    // 形状情報描画(非透明)
    for (const auto& obj : ObjList)
    {
        obj->DrawControlPointsAndLines(); // 制御点描画
        obj->DrawFirstDiffVectors(); // 接線描画
        obj->DrawSecondDiffVectors(); // 2階微分ベクトル描画
        obj->DrawBox(); // ミニマクスボックス描画
        obj->DrawNormalVectors(); // 法線描画
        obj->DrawCurvatureVectors(); // 曲率ベクトル描画
    }
    // 形状描画
    // α値がある場合を考慮して最後に描く
    // TODO: 形状同士のαでの順番考慮
    for (const auto& obj : ObjList)
    {
        obj->Draw(); // モデル描画
    }

    for (auto it = ObjList.begin(), end = ObjList.end(); it != end; it++)
    {
        // 削除フラグチェック
        if ((*it)->IsDeleteFlagRaised())
        {
            delete *it;
            (*it) = NULL;
        }
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

// すべてのMMボックスを囲むボックスを取得する
Box Scene::GetCoverBound()
{
    vector<Box> allBound;

    for (auto it = ObjList.begin(); it != ObjList.end(); ++it)
        allBound.push_back((*it)->GetBound());

    return Box(allBound);
}

Scene::~Scene()
{
    for (auto it = ObjList.begin(); it != ObjList.end(); it++)
        delete *it;
}