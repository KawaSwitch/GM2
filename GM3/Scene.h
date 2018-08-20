#pragma once

#include "Object.h"
#include "Box.h"
#include <list>
#include <map>
#include <memory>

class Object;

class Scene
{
private:
  std::map<std::string, shared_ptr<Object>> _objTable;

public:
  Scene();

  void AddObject(std::string, shared_ptr<Object>);
  void DeleteObjectAll();
  void DeleteObjectEnd();

  void ToggleDrawControlPoints();
  void ToggleDrawFirstDiffVectors();
  void ToggleDrawSecondDiffVectors();
  void ToggleDrawBox();
  void ToggleDrawNormalVectors();
  void ToggleDrawCurvatureVectors();

  void Draw();
  void DrawForPick();

  Box GetCoverBound();

  ~Scene();
};
