cmake_minimum_required(VERSION 2.8)

project(GeometricModelingLinux CXX)
set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Wno-unknown-pragmas")

add_executable(geoModeling
  Main.cpp
  Reader.cpp
  Writer.cpp
  Curve.cpp
  BezierCurve.cpp
  BsplineCurve.cpp
  NurbsCurve.cpp
  Surface.cpp
  BezierSurface.cpp
  BsplineSurface.cpp
  NurbsSurface.cpp
  Axis.cpp
  CalcUtil.cpp
  Camera.cpp
  Color.cpp
  Display.cpp
  DrawUtil.cpp
  EPS.cpp
  glUtil.cpp
  Initialize.cpp
  Keyboard.cpp
  Mouse.cpp
  Picking.cpp
  Quaternion.cpp
  Resize.cpp
  Scene.cpp
  Test.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLUE_INCLUDE_DIRS} )

target_link_libraries(geoModeling ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )
