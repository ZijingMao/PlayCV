# cmakelists: play with opencv algorithms

cmake_minimum_required (VERSION 3.8)
project ("PlayCV" VERSION 0.0.1)

# version
configure_file (version.h version.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(OpenCV REQUIRED PATHS "D:\\Develop\\OpenCV-MinGW\\x64\\mingw\\lib")
message(STATUS "OpenCV Path: \t" ${OpenCV_INCLUDE_DIRS})

# allow debugging
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories (${OpenCV_INCLUDE_DIRS})

add_subdirectory (CVUtils)
add_test (NAME cv_test COMMAND cv_test)

add_subdirectory (Testcases)
add_test (NAME optical_flow_test COMMAND optical_flow_test)