cmake_minimum_required(VERSION 3.10)

project(CVUtils)

find_package(OpenCV REQUIRED PATHS "D:\\Develop\\OpenCV-MinGW\\x64\\mingw\\lib")

include_directories (${OpenCV_INCLUDE_DIRS})

add_library (cv_utils cv_handler.cpp cv_library.cpp cv_manager.cpp)
target_include_directories (cv_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cv_test test.cpp cv_handler.cpp cv_library.cpp cv_manager.cpp)
target_link_libraries (cv_test ${OpenCV_LIBS})
