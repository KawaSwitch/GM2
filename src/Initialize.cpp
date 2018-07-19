#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "GeoGrid.h"
#include "Model.h"
#include "Triangle.h"
#include "Reader.h"
#include "Axis.h"
#include "Initialize.h"

extern NormalAxis* axis;
extern GeoGrid2D* grid;
extern Scene* scene;
extern Scene* test_scene;

const int grid_length = 200;
const int grid_interval = 5;

void InitScene()
{
    scene = new Scene();
    test_scene = new Scene(); // �e�X�g�p�������ŏ�����

    // @�t�@�C���ǂݍ���
    auto reader = new KjsReader("KJS_FILE/");
    auto objs = reader->GetObjectsFromKjsFolder();

    for (unsigned int i = 0; i < objs.size(); i++)
        scene->AddObject(objs[i]);
}

void InitQuaternion()
{
    // ��]�s��
    for (int i = 0; i < 16; i++)
        rot_mat[i] = 0.0;

    // �N�H�[�^�j�I��
    target.x = 0.0; target.y = 0.0;
    target.z = 0.0; target.w = 0.0;
    current.x = 0.0; current.y = 0.0;
    current.z = 0.0; current.w = 1.0;

    // �N�H�[�^�j�I��������
    CalcRotateMatrix(rot_mat, current);
}

void Initialize()
{
    glewInit(); // glew�g��

    glClearColor(1.0, 1.0, 1.0, 1.0);   // �w�i�F�F��
    glClearStencil(0); // �X�e���V���l��0�ŏ�����

    glEnable(GL_DEPTH_TEST); // �f�v�X�l �L����
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND); // �u�����h �L����
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ���C�����C�g������
    {
        lights.push_back(new Light(GL_LIGHT0, 50, 50, -50));
        lights.push_back(new Light(GL_LIGHT1, 50, 0, 50));
        lights.push_back(new Light(GL_LIGHT2, -50, -50, 0));
        lights.push_back(new Light(GL_LIGHT3, 50, 50, 50));
        lights.push_back(new Light(GL_LIGHT4, -50, 0, 0));

        // ���p���C�g�̐ݒ�
        for (const auto light : lights)
            axisLight.push_back(light);
        // �`��p���C�g�̐ݒ�
        {
            modelLight.push_back(lights[0]);
            modelLight.push_back(lights[1]);
        }

        isUseLight = true; // �K�v�ȂƂ���̂ݏ������{��
    }

    // �O���b�h������
    gridType = GridType::WithAxis;
    grid = new GeoGrid2D(grid_length, grid_interval);

    // ��������
    axisShowType = AxisShowType::Normal;
    axis = new NormalAxis(10); // ��

    obj_number = 1; // �������ʎq��1

    // ��]�ړ��n������
    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;
    InitQuaternion();

    glShadeModel(GL_SMOOTH);

    isViewInitRequested = false; // �s�񌳂ɖ߂���

    // �V�[��������
    InitScene();

    // �R���\�[���o��
    ShowConsoleDiscription();
}
