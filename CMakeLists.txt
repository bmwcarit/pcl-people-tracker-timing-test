project(people_tracker_test)

cmake_minimum_required(VERSION 2.6)

find_package(PCL REQUIRED)
find_package(catkin REQUIRED COMPONENTS roscpp rosbag)

add_subdirectory(src)
