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

    // ����_�\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawCtrlp();
        it++;
    }
}

void Scene::ToggleDrawFirstDiffVectors()
{
    auto it = ObjList.begin();

    // �ڐ��\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawFisrtDiff();
        it++;
    }
}

void Scene::ToggleDrawSecondDiffVectors()
{
    auto it = ObjList.begin();

    // 2�K�����x�N�g���\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawSecondDiff();
        it++;
    }
}

void Scene::ToggleDrawBox()
{
    auto it = ObjList.begin();

    // �~�j�}�N�X�{�b�N�X�\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawBox();
        it++;
    }
}

void Scene::ToggleDrawNormalVectors()
{
    auto it = ObjList.begin();

    // �@���\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawNormal();
        it++;
    }
}

void Scene::ToggleDrawCurvatureVectors()
{
    auto it = ObjList.begin();

    // �ȗ��x�N�g���\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawCurvature();
        it++;
    }
}

void Scene::Draw()
{
    // ���l������ꍇ���l�����čŌ�ɕ`��
    // �`����`��(�񓧖�)
    for (const auto& obj : ObjList)
    {
        obj->DrawControlPointsAndLines(); // ����_�`��
        obj->DrawFirstDiffVectors(); // �ڐ��`��
        obj->DrawSecondDiffVectors(); // 2�K�����x�N�g���`��
        obj->DrawBox(); // �~�j�}�N�X�{�b�N�X�`��
        obj->DrawNormalVectors(); // �@���`��
        obj->DrawCurvatureVectors(); // �ȗ��x�N�g���`��
    }
    // �`��`��
    // �`�󓯎m�̃��ł̏��ԍl��
    for (const auto& obj : ObjList)
    {
        if (!obj->IsSemiTransparent())
            obj->Draw(); // ���f���`��
    }
    for (const auto& obj : ObjList)
    {
        if (obj->IsSemiTransparent())
            obj->Draw(); // ���f���`��
    }

    for (auto it = ObjList.begin(), end = ObjList.end(); it != end; it++)
    {
        // �폜�t���O�`�F�b�N
        if ((*it)->IsDeleteFlagRaised())
        {
            delete *it;
            (*it) = NULL;
        }
    }

    ObjList.remove(NULL);
}

// �}�E�X�s�b�L���O�p�`��
void Scene::DrawForPick()
{
    auto it = ObjList.begin();

    // ���X�g�S�`��
    while (it != ObjList.end())
    {
        glLoadName((*it)->GetObjectNumber());
        (*it)->DrawAsItIs();

        it++;
    }
}

// ���ׂĂ�MM�{�b�N�X���͂ރ{�b�N�X���擾����
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