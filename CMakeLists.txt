project(people_tracker_test)

cmake_minimum_required(VERSION 2.6)

find_package(PCL REQUIRED)
find_package(catkin REQUIRED COMPONENTS roscpp rosbag)

add_definitions(-DPCL_PEOPLE_WITHOUT_VISUALIZATION)
add_subdirectory(src)