cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20) # Compile as C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(k-dtree
        VERSION 0.1.0
        DESCRIPTION "A simple implementation of a k-dimensional tree"
        LANGUAGES C CXX
)

find_package(Catch2 REQUIRED)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE lib/)

add_library(kdtree
  src/kdtree.cpp
)

target_include_directories(kdtree PUBLIC src/)
target_link_libraries(kdtree PUBLIC stb)

add_executable(main src/main.cpp src/stb.cpp)
target_link_libraries(main PRIVATE kdtree)

add_executable(tests src/kdtree_test.cpp)
target_link_libraries(tests PRIVATE kdtree Catch2::Catch2WithMain)

