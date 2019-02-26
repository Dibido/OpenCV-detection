cmake_minimum_required(VERSION 2.8)
project(shapedetector)

set(CMAKE_CXX_CPPCHECK "cppcheck")

set (CMAKE_CXX_STANDARD 14)
find_package(OpenCV 3.2.0 REQUIRED)

add_executable(shapedetector main.cpp DetectColor.cpp DetectShapes.cpp Shapedetector.cpp )
target_link_libraries(shapedetector ${OpenCV_LIBS})

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(shapedetector PRIVATE "-Wall")
    target_compile_options(shapedetector PRIVATE "-Wextra")
    target_compile_options(shapedetector PRIVATE "-Wconversion")
endif()
if ( MSVC )
    target_compile_options(shapedetector PRIVATE "/W4")
endif()