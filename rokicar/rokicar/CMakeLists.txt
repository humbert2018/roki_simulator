cmake_minimum_required(VERSION 2.8.3)
project(rokicar)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  perception_msgs
  hadmap_navigation
  hadmap_msgs
  visualization_msgs
  urdf
  nav_msgs
)

add_message_files(
  FILES
  RokiCreatorMsg.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(
  CATKIN_DEPENDS
  message_runtime
#  INCLUDE_DIRS include
#  LIBRARIES rokicar
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

include_directories(
  include ${catkin_INCLUDE_DIRS}
  include ${catkin_INCLUDE_DIRS}
  include include/rokicar
)

add_executable(RokiSys
  src/SimuManager.cpp
  include/rokicar/SimuManager.hpp
  include/rokicar/RokiSystem.hpp
  include/rokicar/BaseRoboCar.hpp
  include/rokicar/RoadInfo.hpp
  include/rokicar/Environment.hpp
  include/rokicar/RokiHeader.h
  include/rokicar/AI/InitAllAIs.hpp
  include/rokicar/AI/simple_demo.hpp
  src/AIManager.cpp src/BaseRoboCar.cpp
  src/Environment.cpp src/RoadInfo.cpp
  src/RokiSystem.cpp
  src/RokiSysMain.cpp)
add_dependencies(RokiSys perception_msgs_gencpp hadmap_navigation_gencpp)
target_link_libraries(RokiSys ${catkin_LIBRARIES})
