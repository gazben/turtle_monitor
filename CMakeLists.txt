cmake_minimum_required(VERSION 2.8.3)
project(turtle_monitor)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(catkin REQUIRED COMPONENTS roscpp turtlesim)

catkin_package()

include_directories(
  INCLUDE_DIRS include include/turtle_monitor
  ${catkin_INCLUDE_DIRS}
)

add_executable(mission_cntrl_groovy src/mission_cntrl_groovy.cpp)
add_executable(vel_monitor src/vel_monitor.cpp)


target_link_libraries(mission_cntrl_groovy ${catkin_LIBRARIES})
target_link_libraries(vel_monitor ${catkin_LIBRARIES})



