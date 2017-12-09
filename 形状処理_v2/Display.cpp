#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Background.h"
#include "Axis.h"
#include "ControlPoint.h"
#include "BsplineCurve.h"
#include "Test.h"
#include "Picking.h"

NormalAxis* axis; // 軸
Scene* scene; // シーン
extern Scene* test_scene;

extern Point3d center;

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 軸描画
    // 軸操作ではモデルビュー行列を変えない
    glPushMatrix();
    glTranslated(-2.0, -2.0, -2.0);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    // 移動
    glTranslated(dist_X, -dist_Y, dist_Z);

    // 回転
    //glTranslated(center.X, center.Y, center.Z);
    glMultMatrixd(rot_mat);
    //glTranslated(-center.X, -center.Y, -center.Z);
    
    //scene_mat = Matrix3d::CreateIdentity();
    //glMultMatrixd(scene_mat);
    //glLoadMatrixd(scene_mat);

    //printf("回転中心:%f %f %f\n", center.X, center.Y, center.Z);

    //// 回転中心描画
    //glColor3dv(Color::red);
    //glPointSize(10.0);
    //glBegin(GL_POINTS);
    //glVertex3d(center.X, center.Y, center.Z);
    //glEnd();

    // 形状描画
    scene->Draw();

    // テスト描画
    TestDraw();

    // 幾何グリッド描画
    //DrawGrid(GeoGrid2D());

    glutSwapBuffers();
}