cmake_minimum_required(VERSION 3.26)
project(bst)

set(CMAKE_CXX_STANDARD 17)

add_executable(bst main.cpp
        bst_tree.hpp)
