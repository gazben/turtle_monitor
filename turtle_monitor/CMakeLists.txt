cmake_minimum_required(VERSION 2.8.3)
project(turtle_monitor)


find_package(catkin REQUIRED COMPONENTS roscpp turtlesim)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(mission_cntrl_groovy mission_cntrl_groovy.cpp)
target_link_libraries(mission_cntrl_groovy ${catkin_LIBRARIES})

add_executable(vel_monitor vel_monitor.cpp)
target_link_libraries(vel_monitor ${catkin_LIBRARIES})



