cmake_minimum_required(VERSION 2.8)

project(GeometricModelingLinux CXX)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s -DNDEBUG -march=native")

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
