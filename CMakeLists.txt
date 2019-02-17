cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-Wall")
set (CMAKE_CXX_FLAGS "-Wextra")
set (CMAKE_CXX_FLAGS "-Wconversion")

project(shapedetector)
find_package(OpenCV 3.2.0 REQUIRED)

add_executable(shapedetector main.cpp Shapedetector.cpp)
target_link_libraries(shapedetector ${OpenCV_LIBS})