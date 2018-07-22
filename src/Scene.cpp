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

    // ����_�\���g�O��
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawCtrlp();
        it++;
    }
}

void Scene::ToggleDrawFirstDiffVectors()
{
    auto it = _objTable.begin();

    // �ڐ��\���g�O��
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawFisrtDiff();
        it++;
    }
}

void Scene::ToggleDrawSecondDiffVectors()
{
    auto it = _objTable.begin();

    // 2�K�����x�N�g���\���g�O��
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawSecondDiff();
        it++;
    }
}

void Scene::ToggleDrawBox()
{
    auto it = _objTable.begin();

    // �~�j�}�N�X�{�b�N�X�\���g�O��
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawBox();
        it++;
    }
}

void Scene::ToggleDrawNormalVectors()
{
    auto it = _objTable.begin();

    // �@���\���g�O��
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawNormal();
        it++;
    }
}

void Scene::ToggleDrawCurvatureVectors()
{
    auto it = _objTable.begin();

    // �ȗ��x�N�g���\���g�O��
    while (it != _objTable.end())
    {
        (*it).second->SetUnsetIsDrawCurvature();
        it++;
    }
}

void Scene::Draw()
{
// ���l������ꍇ���l�����čŌ�ɕ`��
    // �`����`��(�񓧖�)
  for (const auto& obj : _objTable)
    {
        obj.second->DrawControlPointsAndLines(); // ����_�`��
        obj.second->DrawFirstDiffVectors(); // �ڐ��`��
        obj.second->DrawSecondDiffVectors(); // 2�K�����x�N�g���`��
        obj.second->DrawBox(); // �~�j�}�N�X�{�b�N�X�`��
        obj.second->DrawNormalVectors(); // �@���`��
        obj.second->DrawCurvatureVectors(); // �ȗ��x�N�g���`��
    }
    // �`��`��
    // �`�󓯎m�̃��ł̏��ԍl��
  for (const auto& obj : _objTable)
    {
        if (!obj.second->IsSemiTransparent())
            obj.second->Draw(); // ���f���`��
    }
  for (const auto& obj : _objTable)
    {
        if (obj.second->IsSemiTransparent())
            obj.second->Draw(); // ���f���`��
    }

    // for (auto it = ObjList.begin(), end = ObjList.end(); it != end; it++)
    // {
    //     // �폜�t���O�`�F�b�N
    //     if ((*it)->IsDeleteFlagRaised())
    //     {
    //         delete *it;
    //         (*it) = NULL;
    //     }
    // }

    // ObjList.remove(NULL);
}

// �}�E�X�s�b�L���O�p�`��
void Scene::DrawForPick()
{
    auto it = _objTable.begin();

    // ���X�g�S�`��
    while (it != _objTable.end())
    {
        glLoadName((*it).second->GetObjectNumber());
        (*it).second->DrawAsItIs();

        it++;
    }
}

// ���ׂĂ�MM�{�b�N�X���͂ރ{�b�N�X���擾����
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
