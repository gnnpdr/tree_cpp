cmake_minimum_required(VERSION 3.10)
project(RBTree_Tests VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rbtree_tests
    main.cpp
)