cmake_minimum_required(VERSION 3.10)

project(Testcases)

find_package(OpenCV REQUIRED PATHS "D:\\Develop\\OpenCV-MinGW\\x64\\mingw\\lib")

include_directories (${OpenCV_INCLUDE_DIRS})
include_directories ("${PROJECT_SOURCE_DIR}/CVUtils")

add_executable(optical_flow_test optical_flow_test.cpp)
target_link_libraries (optical_flow_test ${OpenCV_LIBS})
target_link_libraries (optical_flow_test cv_utils)

add_executable(color_segment_test color_segment_test.cpp)
target_link_libraries (color_segment_test ${OpenCV_LIBS})
target_link_libraries (color_segment_test cv_utils)
