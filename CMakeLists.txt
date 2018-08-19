cmake_minimum_required(VERSION 2.8)

project(GeometricModelingLinux CXX)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s -DNDEBUG -march=native")

add_executable(geoModeling
  src/Main.cpp
  src/Reader.cpp
  src/Writer.cpp
  src/Curve.cpp
  src/BezierCurve.cpp
  src/BsplineCurve.cpp
  src/NurbsCurve.cpp
  src/Surface.cpp
  src/BezierSurface.cpp
  src/BsplineSurface.cpp
  src/NurbsSurface.cpp
  src/Axis.cpp
  src/CalcUtil.cpp
  src/Camera.cpp
  src/Color.cpp
  src/Display.cpp
  src/DrawUtil.cpp
  src/EPS.cpp
  src/glUtil.cpp
  src/Initialize.cpp
  src/Keyboard.cpp
  src/Mouse.cpp
  src/Picking.cpp
  src/Quaternion.cpp
  src/Resize.cpp
  src/Scene.cpp
  src/Intersection.cpp
  src/Test.cpp)


# OpenGL Settings
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLUE_INCLUDE_DIRS} )

target_link_libraries(geoModeling ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )


# Test if linux, mac or windows : 
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("You're on Mac OS !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("You're on Linux !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("You're on Windows !")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Yes this is weird but you have to repeat the "if" here.
