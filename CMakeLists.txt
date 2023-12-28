cmake_minimum_required(VERSION 3.0.0)
set(CXX_STANDARD 20)
project(Proj VERSION 0.1.0 LANGUAGES C CXX)

add_library(Lib bst.cpp rbtree.cpp)
add_executable(Proj main.cpp)

target_compile_options(Lib PRIVATE -O3)
target_compile_options(Proj PRIVATE -O3)

target_link_libraries(Proj Lib)
