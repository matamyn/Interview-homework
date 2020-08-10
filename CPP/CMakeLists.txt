cmake_minimum_required(VERSION 3.12)
project(CPP_homework)
SET(CMAKE_CXX_FLAGS -pthread)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_subdirectory(question_on_algorithms)
add_subdirectory(common_cpp)
add_subdirectory(StarStaff)
add_subdirectory(kasper)

add_executable(poligon poligon.cpp)